#include "MainWindow.h"
#include "OccView.h"
#include "Document.h"
#include "OccView.h"

#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QStackedWidget>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
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

    m_documents = menuBar()->addMenu("&Documents");
    m_documents->addAction("<No document>")->setEnabled(false);

    m_modeling = addToolBar("Modeling");
    m_makeBottle = m_modeling->addAction(QPixmap(":/icons/Bottle.png"), "&Make bottle");
    connect(m_makeBottle, &QAction::triggered, this, &MainWindow::onMakeBottle);

    m_visualization = addToolBar("Visualization");
    m_fitAll = m_visualization->addAction(QPixmap(":/icons/FitAll.png"), "Fit &All");
    connect(m_fitAll, &QAction::triggered, this, &MainWindow::onFitAll);

    setActions();
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
    int i, j, k = 1;

    // hide <No document> menu entry
    m_documents->actions().at(0)->setVisible(false);

    // look for a unique new document title
    for(i = 1; i < m_documents->actions().length(); i++) {
        Document* document = m_documents->actions().at(i)->data().value<Document*>();
        if(sscanf(document->title().toUtf8().constData(), "Untitled %d", &j)) {
            if(j >= k) k = j + 1;
        }
    }

    m_document = new Document(QString("Untitled %1").arg(k), this);
    connect(m_document, &Document::error, this, &MainWindow::onError);

    // store new document in menu entry data
    QAction* action = m_documents->addAction(m_document->title());
    action->setCheckable(true);
    action->setChecked(true);
    action->setData(QVariant::fromValue<Document*>(m_document));
    connect(action, &QAction::triggered, this, &MainWindow::onDocument);

    setWindowTitle(m_document->title());
    m_stack->addWidget(m_document->widget());
    m_stack->setCurrentWidget(m_document->widget());

    setActions();
}

void MainWindow::onClose()
{
    QAction* action = NULL;

    // look for the corresponding menu action
    for(int i = 1; i < m_documents->actions().length(); i++) {
        if(m_document == m_documents->actions().at(i)->data().value<Document*>()) {
            action = m_documents->actions().at(i);
            break;
        }
    }

    m_documents->removeAction(action);
    m_stack->removeWidget(m_document->widget());
    delete m_document->widget();
    delete m_document;
    m_document = NULL;

    // look for the new current document
    for(int i = 1; i < m_documents->actions().length(); i++) {
        m_document = m_documents->actions().at(i)->data().value<Document*>();
        if(m_document->widget() == m_stack->currentWidget()) {
            break;
        }
    }

    QString title = (m_document) ? m_document->title() : "Kunie";
    setWindowTitle(title);

    setActions();
}

void MainWindow::onDocument()
{
    m_document = qobject_cast<QAction*>(sender())->data().value<Document*>();
    setWindowTitle(m_document->title());
    m_stack->setCurrentWidget(m_document->widget());
}

void MainWindow::onMakeBottle()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_document->makeBottle();
    QApplication::restoreOverrideCursor();
}

void MainWindow::onFitAll()
{
    if(m_document) m_document->view()->fitAll();
}

void MainWindow::onError(const QString &msg)
{
    QApplication::restoreOverrideCursor();
    QMessageBox::critical(this, "Error", msg);
}

void MainWindow::setActions()
{
    bool enabled = m_document != NULL;
    m_close->setEnabled(enabled);
    m_import->setEnabled(enabled);
    m_makeBottle->setEnabled(enabled);
    m_fitAll->setEnabled(enabled);
}
