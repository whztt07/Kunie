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

    void onNew();
    void onClose();
    void onImport();
    void onDocument();
    void onMakeBottle();

    void onFitAll();
    void onError(const QString& msg);

private:
    void updateActions();

    QTabWidget* m_tab;

    Application* m_app;
    Document* m_document;

    QMenu* m_file;
    QAction* m_close;
    QAction* m_import;
    QAction* m_separator;
    QActionGroup* m_documents;

    QMenu* m_view;

    QToolBar* m_modeling;
    QAction* m_makeBottle;

    QToolBar* m_visualization;
    QAction* m_fitAll;
};

#endif // MAINWINDOW_H
