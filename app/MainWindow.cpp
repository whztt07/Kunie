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
#include <QTabWidget>
#include <QStandardPaths>
#include <QSplitter>
#include <QTreeView>

MainWindow::MainWindow(Application *app):
    m_app(app)
{
    setWindowTitle("Kunie");
    resize(1366, 768);

    QSplitter* splitter = new QSplitter(this);

    m_pages = new QTabWidget;
    m_pages->setDocumentMode(true);
    m_pages->setTabsClosable(true);
    m_pages->setMovable(true);
    m_pages->setTabBarAutoHide(true);
    m_pages->setTabPosition(QTabWidget::South);
    connect(m_pages, &QTabWidget::tabCloseRequested, this, &MainWindow::onCloseRequested);
    connect(m_pages, &QTabWidget::currentChanged, this, &MainWindow::updateActions);

    m_treeView = new QTreeView;
    splitter->addWidget(m_treeView);
    splitter->addWidget(m_pages);

    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    setCentralWidget(splitter);

    m_file = menuBar()->addMenu("&File");

    QAction* newDoc = m_file->addAction("&New...");
    newDoc->setShortcuts(QKeySequence::New);
    connect(newDoc, &QAction::triggered, this, &MainWindow::newDocument);

    QAction* open = m_file->addAction("&Open...");
    open->setShortcuts(QKeySequence::Open);
    connect(open, &QAction::triggered, this, &MainWindow::open);

    m_import = m_file->addAction("&Import");
    connect(m_import, &QAction::triggered, this, &MainWindow::import);

    m_file->addSeparator();

    m_save = m_file->addAction("&Save");
    m_save->setShortcuts(QKeySequence::Save);
    connect(m_save, &QAction::triggered, this, &MainWindow::save);

    m_saveAs = m_file->addAction("Save &as...");
    m_saveAs->setShortcuts(QKeySequence::SaveAs);
    connect(m_saveAs, &QAction::triggered, this, &MainWindow::saveAs);

    m_file->addSeparator();

    m_close = m_file->addAction("&Close");
    m_close->setShortcuts(QKeySequence::Close);
    connect(m_close, &QAction::triggered, this, &MainWindow::close);

    QAction* exitAct = m_file->addAction(tr("&Quit"));
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, &QAction::triggered, this, &MainWindow::close);

    m_view = menuBar()->addMenu("&View");
    m_modeling = addToolBar("Modeling");
    m_visualization = addToolBar("Visualization");

    updateActions();
}

MainWindow::~MainWindow()
{

}

Document *MainWindow::currentDocument()
{
    return OccView::document(m_pages->currentWidget());
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
                                         "All (*);;MDTV-Standard (*.std);;BinOcaf (*.cbf);;XmlOcaf (*.xml)");

    if(!file.isEmpty()) currentDocument()->saveAs(file);
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
                                         "All (*);;MDTV-Standard (*.std);;BinOcaf (*.cbf);;XmlOcaf (*.xml)");

    if(!file.isEmpty()) {
        Document* doc = m_app->open(file);
        if(doc) {
            addDocument(doc);
            m_treeView->setModel(new OcafModel(doc));
        }
    }
}

void MainWindow::close()
{
    Document* document = currentDocument();
    m_pages->removeTab(m_pages->currentIndex());
    m_app->closeDocument(document);
}

void MainWindow::onCloseRequested(int index)
{
    Document* document = OccView::document(m_pages->widget(index));
    m_app->closeDocument(document);
    updateActions();
}

void MainWindow::onError(const QString &msg)
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    QMessageBox::critical(this, "Error", msg);
    QApplication::restoreOverrideCursor();
}

void MainWindow::updateActions()
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
    updateActions();
}
