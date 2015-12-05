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
#include <QStackedWidget>

MainWindow::MainWindow(Application *app):
    m_app(app),
    m_document(NULL)
{
    setWindowTitle("Kunie");
    resize(1024, 768);
    m_stack = new QStackedWidget(this);
    m_stack->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    setCentralWidget(m_stack);

    m_file = menuBar()->addMenu("&File");

    QAction* newDoc = m_file->addAction("&New");
    newDoc->setShortcuts(QKeySequence::New);
    connect(newDoc, &QAction::triggered, this, &MainWindow::onNew);

    m_close = m_file->addAction("&Close");
    m_close->setShortcuts(QKeySequence::Close);
    connect(m_close, &QAction::triggered, this, &MainWindow::onClose);

    m_import = m_file->addAction("&Import");
    connect(m_import, &QAction::triggered, this, &MainWindow::onImport);

    QAction* exitAct = m_file->addAction(tr("E&xit"));
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, &QAction::triggered, this, &MainWindow::close);

    m_separator = m_file->addSeparator();
    m_separator->setText("Documents");
    m_documents = new QActionGroup(this);
    m_documents->setExclusive(true);

    m_view = menuBar()->addMenu("&View");
    m_view->setEnabled(false);

    m_modeling = addToolBar("Modeling");
    m_makeBottle = m_modeling->addAction(QPixmap(":/icons/Bottle.png"), "&Make bottle");
    connect(m_makeBottle, &QAction::triggered, this, &MainWindow::onMakeBottle);

    m_visualization = addToolBar("Visualization");
    m_fitAll = m_visualization->addAction(QPixmap(":/icons/FitAll.png"), "Fit &All");
    connect(m_fitAll, &QAction::triggered, this, &MainWindow::onFitAll);

    updateActions();
}

MainWindow::~MainWindow()
{

}

void MainWindow::onImport()
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
        m_document->import(file);
        QApplication::restoreOverrideCursor();
    }
}

void MainWindow::onNew()
{
    QAction* action = NULL;

    m_document = m_app->newDocument();
    connect(m_document, &Document::error, this, &MainWindow::onError);

    // store new document in menu entry data
    action = m_file->addAction(m_document->title());
    m_documents->addAction(action);
    action->setCheckable(true);
    action->setChecked(true);
    action->setData(QVariant::fromValue<Document*>(m_document));
    connect(action, &QAction::triggered, this, &MainWindow::onDocument);

    setWindowTitle(m_document->title());
    m_stack->addWidget(m_document->view()->widget());
    m_stack->setCurrentWidget(m_document->view()->widget());

    updateActions();
}

void MainWindow::onClose()
{
    QAction* action = m_documents->checkedAction();
    m_documents->removeAction(action);
    m_file->removeAction(action);
    m_stack->removeWidget(m_document->view()->widget());
    m_app->closeDocument(m_document);
    m_document = NULL;

    // look for the new current document
    foreach (action, m_documents->actions()) {
        m_document = action->data().value<Document*>();
        if (m_document->view()->widget() == m_stack->currentWidget()) {
            action->setChecked(true);
            break;
        }
    }

    QString title = (m_document) ? m_document->title() : "Kunie";
    setWindowTitle(title);

    updateActions();
}

void MainWindow::onDocument()
{
    m_document = qobject_cast<QAction*>(sender())->data().value<Document*>();
    setWindowTitle(m_document->title());
    m_stack->setCurrentWidget(m_document->view()->widget());
    updateActions();
}

void MainWindow::onMakeBottle()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_document->makeBottle();
    QApplication::restoreOverrideCursor();
}

void MainWindow::onFitAll()
{
    m_document->view()->fitAll();
}

void MainWindow::onError(const QString &msg)
{
    QApplication::restoreOverrideCursor();
    QMessageBox::critical(this, "Error", msg);
}

void MainWindow::updateActions()
{
    bool enabled = m_document != NULL;
    m_separator->setVisible(enabled);
    m_close->setEnabled(enabled);
    m_import->setEnabled(enabled);
    m_makeBottle->setEnabled(enabled);
    m_fitAll->setEnabled(enabled);

    m_view->clear();
    m_view->setEnabled(enabled);

    if(m_document)
        m_view->addActions(m_document->view()->widget()->actions());
}
