#include "Application.h"
#include "Document.h"
#include "MainWindow.h"
#include "CylinderDriver.h"
#include "SphereDriver.h"
#include "CutDriver.h"
#include <AppStd_Application.hxx>
#include <TFunction_DriverTable.hxx>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QDir>
#include <QFileInfo>

Application::Application(int &argc, char **argv):
    QApplication(argc, argv)
{
    setAttribute(Qt::AA_NativeWindows);

    initEnv();

    m_ocafApp = new AppStd_Application();
    TFunction_DriverTable::Get()->AddDriver(CylinderDriver::GetID(), new CylinderDriver());
    TFunction_DriverTable::Get()->AddDriver(SphereDriver::GetID(), new SphereDriver());
    TFunction_DriverTable::Get()->AddDriver(CutDriver::GetID(), new CutDriver());

    m_window = new MainWindow(this);
    m_window->show();
    m_window->newDocument();
}

MainWindow *Application::window()
{
    return m_window;
}

Document* Application::newDocument(const QString& title)
{
    char name[64];
    int i, j = 1;

    // look for a unique new document title
    foreach(Document* doc, m_documents) {
        if (sscanf(doc->title().toUtf8().constData(), "%64s <%d>", name, &i) == 2) {
            if (i >= j)
                j = i + 1;
        }
    }

    m_documents.append(new Document(QString("%1 <%2>").arg(title).arg(j), this));

    return m_documents.last();
}

Document *Application::open(const QString file)
{
    Document* doc = newDocument(QFileInfo(file).fileName());
    if(doc->open(file))
        return doc;
    else {
        closeDocument(doc);
        return NULL;
    }
}

void Application::closeDocument(Document *doc)
{
    if(m_documents.removeOne(doc)) {
        m_ocafApp->Close(doc->ocafDoc());
        delete doc;
    }
}

Handle(TDocStd_Application) Application::ocafApp()
{
    return m_ocafApp;
}

void Application::wait(int ms)
{
    QElapsedTimer timer;
    timer.start();
    do {
        processEvents(QEventLoop::AllEvents, ms);
    } while (timer.elapsed() < ms);
}

void Application::initEnv()
{
    QByteArray CASROOT = qgetenv("CASROOT");
    QByteArray StdResource = CASROOT + QDir::separator().toLatin1() + "src" + QDir::separator().toLatin1() + "StdResource";
    QByteArray CSF_PluginDefaults = qgetenv("CSF_PluginDefaults");
    QByteArray CSF_StandardDefaults = qgetenv("CSF_StandardDefaults");

    if(CASROOT.isEmpty()) {
        QMessageBox::critical(NULL, applicationName(), "CASROOT not set");
        ::exit(EXIT_FAILURE);
    }

    if(CSF_PluginDefaults.isEmpty()) {
        CSF_PluginDefaults = StdResource + QDir::separator().toLatin1() + "Plugin";
        qputenv("CSF_PluginDefaults", StdResource);
    }

    if(CSF_StandardDefaults.isEmpty()) {
        CSF_StandardDefaults = StdResource + QDir::separator().toLatin1() + "Standard";
        qputenv("CSF_StandardDefaults", StdResource);
    }

    if(!QFileInfo(CSF_PluginDefaults).isReadable()) {
        QMessageBox::critical(NULL, applicationName(), CSF_PluginDefaults + " is not readable");
        ::exit(EXIT_FAILURE);
    }

    if(!QFileInfo(CSF_StandardDefaults).isReadable()) {
        QMessageBox::critical(NULL, applicationName(), CSF_StandardDefaults + " is not readable");
        ::exit(EXIT_FAILURE);
    }
}
