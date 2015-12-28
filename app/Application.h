#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QList>
#include <XCAFApp_Application.hxx>

class Document;
class MainWindow;

class Application : public QApplication
{
public:
    Application(int& argc, char** argv);
    Handle(XCAFApp_Application) ocafApp();
    MainWindow* window();
    Document* newDocument(const QString& title = "Untitle");
    Document* open(const QString file);
    void closeDocument(Document* doc);

    void static wait(int ms);

private:
    void initEnv();

    Handle(XCAFApp_Application) m_ocafApp;
    QList<Document*> m_documents;
    MainWindow* m_window;
};

#endif // APPLICATION_H
