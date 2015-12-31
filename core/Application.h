#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QList>
#include <TDocStd_Application.hxx>

class Document;
class MainWindow;

class Application : public QApplication
{
    Q_OBJECT

public:
    Application(int& argc, char** argv);
    ~Application();

    MainWindow* window();
    Document* newDoc();
    Document* openDoc(const QString& file);
    void closeDoc(Document* doc);

    void static wait(int ms);

signals:
    void error(const QString& msg);

private:
    void initEnv();

    Handle(TDocStd_Application) m_ocafApp;
    QList<Document*> m_documents;
    MainWindow* m_window;
};

#endif // APPLICATION_H
