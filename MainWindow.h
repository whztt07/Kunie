#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Document;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

private:
    Document* m_document;
};

#endif // MAINWINDOW_H
