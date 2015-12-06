#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Application;
class Document;
class QTabWidget;
class QActionGroup;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Application* app);
    ~MainWindow();

    Document* currentDocument();

    void newDocument();
    void close();
    void import();
    void createBottle();

    void fitAll();

private:
    void onCloseRequested(int index);
    void onCurrentChanged();

    void onError(const QString& msg);

    void updateActions();

    QTabWidget* m_pages;

    Application* m_app;

    QMenu* m_file;
    QAction* m_close;
    QAction* m_import;

    QMenu* m_view;

    QToolBar* m_modeling;
    QAction* m_makeBottle;

    QToolBar* m_visualization;
    QAction* m_fitAll;
};

#endif // MAINWINDOW_H
