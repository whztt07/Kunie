#include "MainWindow.h"
#include "Application.h"
#include "Document.h"
#include "OccView.h"

#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QStackedWidget>
#include <QTabWidget>
#include <QStandardPaths>
#include <QSplitter>
#include <QTreeWidget>
#include <QSettings>
#include <QStatusBar>
#include <QDebug>


MainWindow::MainWindow(Application *app):
    m_app(app)
{
    setWindowTitle(qApp->applicationName());
    connect(app, &Application::error, this, &MainWindow::onError);

    QSplitter* splitter = new QSplitter(this);

    m_pages = new QTabWidget(splitter);
    m_pages->setDocumentMode(true);
    m_pages->setTabsClosable(true);
    m_pages->setMovable(true);
    m_pages->setTabBarAutoHide(true);
    m_pages->setTabPosition(QTabWidget::South);
    connect(m_pages, &QTabWidget::tabCloseRequested, this, &MainWindow::onCloseRequested);
    connect(m_pages, &QTabWidget::currentChanged, this, &MainWindow::onCurrentChanged);

    m_trees = new QStackedWidget(splitter);

    splitter->addWidget(m_trees);
    splitter->addWidget(m_pages);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    setCentralWidget(splitter);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    readSettings();

    newDoc();
}

MainWindow::~MainWindow()
{
}

Document* MainWindow::currentDoc()
{
    return OccView::document(m_pages->currentWidget());
}

bool MainWindow::setCurrentDoc(Document* doc)
{
    if(documentIndex(doc) == -1)
        return false;

    m_pages->setCurrentWidget(doc->view()->widget());
    return true;
}


Document *MainWindow::newDoc()
{
    Document* doc = m_app->newDoc();
    addDocument(doc);
    return doc;
}

Document* MainWindow::openDoc(const QString& file)
{
    Document* doc = m_app->openDoc(file);
    if (doc) {
        if (setCurrentDoc(doc)) {
            statusBar()->showMessage(doc->name() + " already loaded", 2000);
            return doc;
        } else {
            Document* prevDoc = currentDoc();
            addDocument(doc);
            doc->view()->fitAll();
            statusBar()->showMessage(doc->name() + " loaded", 2000);

            if(prevDoc && !prevDoc->ocafDoc()->IsSaved() && !prevDoc->ocafDoc()->IsModified())
                closeDoc(prevDoc);
        }
    }
    return doc;
}

bool MainWindow::closeDoc(Document *doc)
{
    int index = documentIndex(doc);
    if (index >= 0) {
        onCloseRequested(index);
        return true;
    }

    return false;
}

bool MainWindow::closeAll()
{
    while(currentDoc()) {
        if (!closeDoc(currentDoc()))
            return false;
    }
    return true;
}

void MainWindow::open()
{
    QString file =
            QFileDialog::getOpenFileName(this, "Open", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                         "All (*);;MDTV-XCAF (*.dxc);;XmlXCAF (*.xml);;BinXCAF (*.xbf);;"
                                         "MDTV-Standard (*.std);;BinOcaf (*.cbf);;IGES (*.igs *.iges);;STEP (*.stp *.step)");

    if(!file.isEmpty()) openDoc(file);
}

void MainWindow::import()
{
    QString file =
            QFileDialog::getOpenFileName(this, "Import", qgetenv("CASROOT") + "/data",
                                         "All (*);;BREP (*.brep *.rle);;IGES (*.igs *.iges);;STEP (*.stp *.step);;"
                                         "CSFDB (*.csfdb);;VRML (*.vrml);;STL (*.stl)");

    if(!file.isEmpty()) currentDoc()->import(file);
}

void MainWindow::save()
{
    saveDocument(currentDoc());
}

void MainWindow::saveAs()
{
    saveDocumentAs(currentDoc());
}

bool MainWindow::saveDocument(Document* doc)
{
    bool success = false;

    if (doc->ocafDoc()->IsSaved()) {
        if(doc->ocafDoc()->IsModified()) {
            success = doc->save();
            if (success)
                statusBar()->showMessage(doc->name() + " saved", 2000);
        }
    } else {
        success = saveDocumentAs(doc);
    }

    return success;
}

int MainWindow::documentIndex(Document *doc)
{
    int res = -1;
    for (int i=0; i<m_pages->count(); i++) {
        if (m_pages->widget(i) == doc->view()->widget())
            res = i;
    }
    return res;
}

bool MainWindow::saveDocumentAs(Document* doc)
{
    QString file =
            QFileDialog::getSaveFileName(this, "Save As", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                         "All (*);;MDTV-XCAF (*.dxc);;XmlXCAF (*.xml);;BinXCAF (*.xbf);;"
                                         "MDTV-Standard (*.std);;BinOcaf (*.cbf)");

    if(!file.isEmpty() && doc->saveAs(file)) {
        int index = documentIndex(doc);
        m_pages->setTabToolTip(index, doc->path());
        m_pages->setTabText(index, doc->name());
        setWindowTitle(qApp->applicationName() + " - " + currentDoc()->name());
        return true;
    }

    return false;
}

void MainWindow::closeEvent(QCloseEvent*)
{
    writeSettings();
}

void MainWindow::onCloseRequested(int index)
{
    QWidget* widget = m_pages->widget(index);
    Document* doc = OccView::document(widget);

    if (maybeSave(doc)) {
        m_pages->removeTab(index);
        delete widget;

        QTreeWidget* tree = doc->tree();
        m_trees->removeWidget(tree);
        delete tree;

        m_app->closeDoc(doc);
    }
}

void MainWindow::onError(const QString &msg)
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    QMessageBox::critical(this, qApp->applicationName(), msg);
    QApplication::restoreOverrideCursor();
}

void MainWindow::onMessage(const QString& msg)
{
    statusBar()->showMessage(msg);
}

void MainWindow::createActions()
{
    m_newDoc = new QAction(QPixmap(":/icons/New.png"), "&New", this);
    m_newDoc->setShortcuts(QKeySequence::New);
    connect(m_newDoc, &QAction::triggered, this, &MainWindow::newDoc);

    m_open = new QAction(QPixmap(":/icons/Open.png"), "&Open...", this);
    m_open->setShortcuts(QKeySequence::Open);
    connect(m_open, &QAction::triggered, this, &MainWindow::open);

    m_import = new QAction("&Import", this);
    connect(m_import, &QAction::triggered, this, &MainWindow::import);

    m_save = new QAction(QPixmap(":/icons/Save.png"), "&Save", this);
    m_save->setShortcuts(QKeySequence::Save);
    connect(m_save, &QAction::triggered, this, &MainWindow::save);

    m_saveAs = new QAction("Save &as...", this);
    m_saveAs->setShortcuts(QKeySequence::SaveAs);
    connect(m_saveAs, &QAction::triggered, this, &MainWindow::saveAs);

    m_close = new QAction("&Close", this);
    m_close->setShortcuts(QKeySequence::Close);
    connect(m_close, &QAction::triggered, this, &MainWindow::close);

    m_exit = new QAction("&Quit", this);
    m_exit->setShortcuts(QKeySequence::Quit);
    connect(m_exit, &QAction::triggered, this, &MainWindow::close);
}

void MainWindow::createMenus()
{
    m_fileMenu = menuBar()->addMenu("&File");
    m_fileMenu->addAction(m_newDoc);
    m_fileMenu->addAction(m_open);
    m_fileMenu->addAction(m_import);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_save);
    m_fileMenu->addAction(m_saveAs);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_close);
    m_fileMenu->addAction(m_exit);

    m_viewMenu = menuBar()->addMenu("&View");
}

void MainWindow::createToolBars()
{
    m_fileToolBar = addToolBar("File");
    m_fileToolBar->addAction(m_newDoc);
    m_fileToolBar->addAction(m_open);
    m_fileToolBar->addAction(m_save);

    m_modeling = addToolBar("Modeling");
    m_visualization = addToolBar("Visualization");
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage("Welcome to Kunié");
}

void MainWindow::readSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(1366,768)).toSize());
    if(settings.contains("pos"))
        move(settings.value("pos").toPoint());
    settings.endGroup();
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
}

bool MainWindow::maybeSave(Document* doc)
{
    qInfo() << doc->name() << doc->ocafDoc()->IsModified() << doc->ocafDoc()->IsSaved();
    if (doc->ocafDoc()->IsModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, qApp->applicationName(),
                                   "The document has been modified.\n"
                                   "Do you want to save your changes ?",
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return saveDocument(doc);
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::onCurrentChanged(int index)
{
    bool enabled = currentDoc() != NULL;
    m_close->setEnabled(enabled);
    m_import->setEnabled(enabled);

    m_save->setEnabled(enabled && currentDoc()->ocafDoc()->IsSaved());
    m_saveAs->setEnabled(enabled);

    m_viewMenu->clear();
    m_viewMenu->setEnabled(enabled);

    m_modeling->clear();
    m_modeling->setVisible(enabled);

    m_visualization->clear();
    m_visualization->setVisible(enabled);

    if(m_pages->currentWidget()) {
        m_viewMenu->addActions(currentDoc()->view()->renderActions()->actions());
        m_modeling->addActions(currentDoc()->modelingActions()->actions());
        m_visualization->addActions(currentDoc()->view()->viewActions()->actions());
        setWindowTitle(qApp->applicationName() + " - " + currentDoc()->name());
    } else {
        setWindowTitle(qApp->applicationName());
    }
}

void MainWindow::addDocument(Document* doc)
{
    connect(doc, &Document::error, this, &MainWindow::onError);

    int index = m_pages->addTab(doc->view()->widget(), uniqueTitle(doc->name()));
    m_pages->setTabToolTip(index, doc->path());
    m_pages->setCurrentWidget(doc->view()->widget());

    m_trees->addWidget(doc->tree());
    m_trees->setCurrentWidget(doc->tree());
}

QString MainWindow::uniqueTitle(const QString& title)
{
    char name[128];
    bool match = false;
    int i, j;

    for (int index = 0; index < m_pages->count(); index++) {
        QString label = m_pages->tabText(index);
        if (sscanf(label.toUtf8().constData(), "%128s", name) == 1) {
            if(title == name) {
                match = true;
                j = 2;
                if (sscanf(label.toUtf8().constData(), "%128s <%d>", name, &i) == 2) {
                    if (i >= j)
                        j = i + 1;
                }
            }
        }
    }

    if (match)
        return QString("%1 <%2>").arg(title).arg(j);

    return title;
}
