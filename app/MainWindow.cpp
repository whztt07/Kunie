#include "MainWindow.h"
#include "Application.h"
#include "OccView.h"
#include "Document.h"
#include "OccView.h"

#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include <QFileDialog>
#include <QMessageBox>
#include <QTabWidget>

MainWindow::MainWindow(Application *app):
    m_app(app)
{
    setWindowTitle("Kunie");
    resize(1024, 768);
    m_pages = new QTabWidget(this);
    m_pages->setDocumentMode(true);
    m_pages->setTabsClosable(true);
    m_pages->setMovable(true);
    m_pages->setTabBarAutoHide(true);
    m_pages->setTabPosition(QTabWidget::South);
    connect(m_pages, &QTabWidget::tabCloseRequested, this, &MainWindow::onCloseRequested);
    connect(m_pages, &QTabWidget::currentChanged, this, &MainWindow::onCurrentChanged);
    setCentralWidget(m_pages);

    m_file = menuBar()->addMenu("&File");

    QAction* newDoc = m_file->addAction("&New");
    newDoc->setShortcuts(QKeySequence::New);
    connect(newDoc, &QAction::triggered, this, &MainWindow::newDocument);

    m_close = m_file->addAction("&Close");
    m_close->setShortcuts(QKeySequence::Close);
    connect(m_close, &QAction::triggered, this, &MainWindow::close);

    m_import = m_file->addAction("&Import");
    connect(m_import, &QAction::triggered, this, &MainWindow::import);

    QAction* exitAct = m_file->addAction(tr("E&xit"));
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, &QAction::triggered, this, &MainWindow::close);

    m_view = menuBar()->addMenu("&View");
    m_view->setEnabled(false);

    m_modeling = addToolBar("Modeling");
    m_makeBottle = m_modeling->addAction(QPixmap(":/icons/Bottle.png"), "&Make bottle");
    connect(m_makeBottle, &QAction::triggered, this, &MainWindow::createBottle);

    m_visualization = addToolBar("Visualization");
    m_fitAll = m_visualization->addAction(QPixmap(":/icons/FitAll.png"), "Fit &All");
    connect(m_fitAll, &QAction::triggered, this, &MainWindow::fitAll);

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
    QString file = QFileDialog::getOpenFileName(this, "Import", qgetenv("CASROOT") + "/data",
                                                "All (*);;"
                                                "BREP (*.brep *.rle);;"
                                                "IGES (*.igs *.iges);;"
                                                "STEP (*.stp *.step);;"
                                                "CSFDB (*.csfdb);;"
                                                "VRML (*.vrml);;"
                                                "STL (*.stl);;"
                                                );

    if(!file.isEmpty()) {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        currentDocument()->import(file);
        QApplication::restoreOverrideCursor();
    }
}

void MainWindow::newDocument()
{
    Document* document = m_app->newDocument();
    connect(document, &Document::error, this, &MainWindow::onError);

    int index = m_pages->addTab(document->view()->widget(), document->title());
    m_pages->setTabToolTip(index, document->title());
    m_pages->setCurrentWidget(document->view()->widget());

    updateActions();
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

void MainWindow::onCurrentChanged()
{    
    updateActions();
}

void MainWindow::createBottle()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    currentDocument()->createCylinder(0, 0, 0, 10, 50);
    QApplication::restoreOverrideCursor();
}

void MainWindow::fitAll()
{
    currentDocument()->view()->fitAll();
}

void MainWindow::onError(const QString &msg)
{
    QApplication::restoreOverrideCursor();
    QMessageBox::critical(this, "Error", msg);
}

void MainWindow::updateActions()
{
    bool enabled = currentDocument() != NULL;
    m_close->setEnabled(enabled);
    m_import->setEnabled(enabled);
    m_makeBottle->setEnabled(enabled);
    m_fitAll->setEnabled(enabled);

    m_view->clear();
    m_view->setEnabled(enabled);

    if(m_pages->currentWidget()) {
        m_view->addActions(m_pages->currentWidget()->actions());
        setWindowTitle(currentDocument()->title());
    } else {
        setWindowTitle("Kunie");
    }
}
