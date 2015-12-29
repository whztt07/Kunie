#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Application;
class Document;
class QStackedWidget;
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
    void open();
    void import();
    void save();
    void saveAs();

protected:
    void closeEvent(QCloseEvent* event);

private:
    void onCloseRequested(int index);
    void onError(const QString& msg);
    void onMessage(const QString& msg);

    void createActions();
    void createMenus();
    void createToolBars();

    void readSettings();
    void writeSettings();
    void onCurrentChanged();
    void addDocument(Document* doc);
    void removeDocument(Document* doc);

    QStackedWidget* m_trees;
    QTabWidget* m_pages;
    Application* m_app;

    QAction* m_newDoc;
    QAction* m_open;
    QAction* m_import;
    QAction* m_save;
    QAction* m_saveAs;
    QAction* m_close;
    QAction* m_exit;

    QMenu* m_file;
    QMenu* m_view;

    QToolBar* m_modeling;
    QToolBar* m_visualization;
};

#endif // MAINWINDOW_H
