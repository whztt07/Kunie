#include "MainWindow.h"
#include "OccView.h"
#include "Document.h"
#include "OccView.h"

#include <QWidget>
#include <QToolBar>
#include <QAction>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent)
{
    m_document = new Document(this);

    setWindowTitle("Kunie");
    setCentralWidget(QWidget::createWindowContainer(m_document->view(), this));
    resize(600, 450);

    m_modeling = addToolBar("Madeling");
    m_makeBottle = m_modeling->addAction(QPixmap("://icons/Bottle.png"), "&Make bottle");
    connect(m_makeBottle, &QAction::triggered, m_document, &Document::onMakeBottle);

    m_visualization = addToolBar("Visualization");
    m_fitAll = m_visualization->addAction(QPixmap(":/icons/FitAll.png"), "&Fit all");
    connect(m_fitAll, &QAction::triggered, m_document->view(), &OccView::fitAll);
}
