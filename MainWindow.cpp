#include "MainWindow.h"
#include "OccView.h"
#include "Document.h"
#include "OccView.h"

#include <QWidget>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent)
{
    m_document = new Document(this);
    connect(m_document, &Document::error, this, &MainWindow::onError);

    setWindowTitle("Kunie");
    setCentralWidget(QWidget::createWindowContainer(m_document->view(), this));
    resize(600, 450);

    m_file = menuBar()->addMenu("&File");
    m_import = m_file->addAction("&Import");
    connect(m_import, &QAction::triggered, this, &MainWindow::onImport);

    m_modeling = addToolBar("Modeling");
    m_makeBottle = m_modeling->addAction(QPixmap(":/icons/Bottle.png"), "&Make bottle");
    connect(m_makeBottle, &QAction::triggered, m_document, &Document::makeBottle);

    m_visualization = addToolBar("Visualization");
    m_fitAll = m_visualization->addAction(QPixmap(":/icons/FitAll.png"), "Fit &All");
    connect(m_fitAll, &QAction::triggered, m_document->view(), &OccView::fitAll);
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

    if(!file.isEmpty()) m_document->import(file);
}

void MainWindow::onError(const QString &msg)
{
    QMessageBox::critical(this, "Error", msg);
}
