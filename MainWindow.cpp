#include "MainWindow.h"
#include "OccView.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent)
{
    setCentralWidget(OccView::create(this));
    resize(600, 450);
}
