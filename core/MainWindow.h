#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Application;
class Document;
class QStackedWidget;
class QTabWidget;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Application* app);
    ~MainWindow();

    Document* currentDoc();
    bool setCurrentDoc(Document* doc);
    Document* newDoc();
    Document* openDoc(const QString& file);
    bool closeDoc(Document* doc);
    bool closeAll();

protected:
    void closeEvent(QCloseEvent* event);

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();

    void readSettings();
    void writeSettings();

    void open();
    void import();
    void save();
    void saveAs();

    void onCurrentChanged(int index);
    void onCloseRequested(int documentIndex);
    void onError(const QString& msg);
    void onMessage(const QString& msg);

    bool saveDocumentAs(Document* doc);
    bool saveDocument(Document* doc);
    void addDocument(Document* doc);
    bool removeDocument(Document* doc);
    int documentIndex(Document* doc);
    bool maybeSave(Document *doc);

    QString uniqueTitle(const QString& title);

    Application* m_app;
    QStackedWidget* m_trees;
    QTabWidget* m_pages;

    QAction* m_newDoc;
    QAction* m_open;
    QAction* m_import;
    QAction* m_save;
    QAction* m_saveAs;
    QAction* m_close;
    QAction* m_exit;

    QMenu* m_fileMenu;
    QMenu* m_viewMenu;

    QToolBar* m_fileToolBar;
    QToolBar* m_modeling;
    QToolBar* m_visualization;
};

#endif // MAINWINDOW_H
