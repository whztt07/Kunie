#include "MainWindow.h"
#include "OccView.h"
#include "Document.h"

#include <QToolBar>
#include <QAction>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent)
{
    m_document = new Document(this);

    setWindowTitle("Kunie");
    setCentralWidget(m_document->createView());
    resize(600, 450);

    QAction* action = addToolBar("Make Bottle")->addAction(QPixmap("://icons/Bottle.png"), "&Make Bottle");
    connect(action, &QAction::triggered, m_document, &Document::onMakeBottle);
}
