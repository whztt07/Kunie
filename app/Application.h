#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QList>
#include <AppStd_Application.hxx>

class Document;
class MainWindow;

class Application : public QApplication
{
public:
    Application(int& argc, char** argv);
    Handle(AppStd_Application) ocafApp();
    MainWindow* window();
    Document* newDocument();
    void closeDocument(Document* doc);

private:
    Handle(AppStd_Application) m_app;
    QList<Document*> m_documents;
    MainWindow* m_window;
};

#endif // APPLICATION_H
