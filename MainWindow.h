#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Document;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

    void onImport();
    void onError(const QString& msg);

private:
    Document* m_document;
    QMenu* m_file;
    QToolBar* m_modeling;
    QToolBar* m_visualization;
    QAction* m_import;
    QAction* m_makeBottle;
    QAction* m_fitAll;
};

#endif // MAINWINDOW_H
