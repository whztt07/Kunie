#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Document;
class QStackedWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

    void onNew();
    void onClose();
    void onImport();
    void onDocument();
    void onMakeBottle();

    void onFitAll();
    void onError(const QString& msg);

private:
    void setActions();

    QStackedWidget* m_stack;
    Document* m_document;
    QMenu* m_file;
    QMenu* m_documents;
    QToolBar* m_modeling;
    QToolBar* m_visualization;
    QAction* m_close;
    QAction* m_import;
    QAction* m_makeBottle;
    QAction* m_fitAll;
};

#endif // MAINWINDOW_H
