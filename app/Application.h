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

    void static wait(int ms);

private:
    Handle(AppStd_Application) m_ocafApp;
    QList<Document*> m_documents;
    MainWindow* m_window;
};

#endif // APPLICATION_H
