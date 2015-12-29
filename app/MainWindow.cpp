#include "MainWindow.h"
#include "Application.h"
#include "OccView.h"
#include "Document.h"
#include "OccView.h"
#include "OcafModel.h"

#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include <QFileDialog>
#include <QMessageBox>
#include <QStackedWidget>
#include <QTabWidget>
#include <QStandardPaths>
#include <QSplitter>
#include <QTreeWidget>
#include <QSettings>
#include <QStatusBar>
#include <QStyle>

MainWindow::MainWindow(Application *app):
    m_app(app)
{
    setWindowTitle(qApp->applicationName());

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
    statusBar()->showMessage("Welcome to Kunié");
    readSettings();

    newDocument();
}

MainWindow::~MainWindow()
{
}

Document *MainWindow::currentDocument()
{
    return OccView::document(m_pages->currentWidget());
}

void MainWindow::newDocument()
{
    Document* doc = m_app->newDocument();
    addDocument(doc);
}

void MainWindow::open()
{
    QString file =
            QFileDialog::getOpenFileName(this, "Open", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                         "All (*);;MDTV-XCAF (*.dxc);;XmlXCAF (*.xml);;BinXCAF (*.xbf);;"
                                         "MDTV-Standard (*.std);;BinOcaf (*.cbf);;IGES (*.igs *.iges);;STEP (*.stp *.step)");

    if(!file.isEmpty()) {
        Document* doc = m_app->open(file);
        if(doc) {
            addDocument(doc);
            doc->view()->fitAll();
        }
    }
}

void MainWindow::import()
{
    QString file =
            QFileDialog::getOpenFileName(this, "Import", qgetenv("CASROOT") + "/data",
                                         "All (*);;BREP (*.brep *.rle);;IGES (*.igs *.iges);;STEP (*.stp *.step);;"
                                         "CSFDB (*.csfdb);;VRML (*.vrml);;STL (*.stl)");

    if(!file.isEmpty()) currentDocument()->import(file);
}

void MainWindow::save()
{

}

void MainWindow::saveAs()
{
    QString file =
            QFileDialog::getSaveFileName(this, "Save As", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                         "All (*);;MDTV-XCAF (*.dxc);;XmlXCAF (*.xml);;BinXCAF (*.xbf);;"
                                         "MDTV-Standard (*.std);;BinOcaf (*.cbf)");

    if(!file.isEmpty()) currentDocument()->saveAs(file);
}

void MainWindow::closeEvent(QCloseEvent*)
{
    writeSettings();
}

void MainWindow::onCloseRequested(int index)
{
    QWidget* widget = m_pages->widget(index);
    Document* document = OccView::document(widget);
    m_pages->removeTab(index);
    delete widget;

    QTreeWidget* tree = document->tree();
    m_trees->removeWidget(tree);
    delete tree;

    m_app->closeDocument(document);
    onCurrentChanged();
}

void MainWindow::onError(const QString &msg)
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    QMessageBox::critical(this, "Error", msg);
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
    connect(m_newDoc, &QAction::triggered, this, &MainWindow::newDocument);

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
    m_file = menuBar()->addMenu("&File");
    m_file->addAction(m_newDoc);
    m_file->addAction(m_open);
    m_file->addAction(m_import);
    m_file->addSeparator();
    m_file->addAction(m_save);
    m_file->addAction(m_saveAs);
    m_file->addSeparator();
    m_file->addAction(m_close);
    m_file->addAction(m_exit);

    m_view = menuBar()->addMenu("&View");
}

void MainWindow::createToolBars()
{
    m_modeling = addToolBar("Modeling");
    m_visualization = addToolBar("Visualization");
}

void MainWindow::readSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(1366,768)).toSize());
    settings.endGroup();
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.endGroup();
}

void MainWindow::onCurrentChanged()
{
    bool enabled = currentDocument() != NULL;
    m_close->setEnabled(enabled);
    m_import->setEnabled(enabled);

    m_view->clear();
    m_view->setEnabled(enabled);

    m_modeling->clear();
    m_modeling->setVisible(enabled);

    m_visualization->clear();
    m_visualization->setVisible(enabled);

    if(m_pages->currentWidget()) {
        m_view->addActions(currentDocument()->view()->renderActions()->actions());
        m_modeling->addActions(currentDocument()->modelingActions()->actions());
        m_visualization->addActions(currentDocument()->view()->viewActions()->actions());
        setWindowTitle(currentDocument()->title());
    } else {
        setWindowTitle("Kunie");
    }
}

void MainWindow::addDocument(Document* doc)
{
    connect(doc, &Document::error, this, &MainWindow::onError);

    int index = m_pages->addTab(doc->view()->widget(), doc->title());
    m_pages->setTabToolTip(index, doc->title());
    m_pages->setCurrentWidget(doc->view()->widget());

    m_trees->addWidget(doc->tree());
    m_trees->setCurrentWidget(doc->tree());
}
