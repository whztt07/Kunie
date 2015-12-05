#include "Application.h"
#include "Document.h"
#include "MainWindow.h"
#include "CylinderDriver.h"
#include <TFunction_DriverTable.hxx>

Application::Application(int &argc, char **argv):
    QApplication(argc, argv)
{
    setAttribute(Qt::AA_NativeWindows);

    m_app = new AppStd_Application();
    TFunction_DriverTable::Get()->AddDriver(CylinderDriver::GetID(), new CylinderDriver());

    m_window = new MainWindow(this);
    m_window->show();
}

MainWindow *Application::window()
{
    return m_window;
}

Document* Application::newDocument()
{
    int i, j = 1;

    // look for a unique new document title
    foreach(Document* doc, m_documents) {
        if (sscanf(doc->title().toUtf8().constData(), "Untitled %d", &i)) {
            if (i >= j)
                j = i + 1;
        }
    }

    m_documents.append(new Document(QString("Untitled %1").arg(j), this));

    return m_documents.last();
}

void Application::closeDocument(Document *doc)
{
    if(m_documents.removeOne(doc))
        delete doc;
}

Handle(AppStd_Application) Application::ocafApp()
{
    return m_app;
}

