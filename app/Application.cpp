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
    QCoreApplication::setOrganizationName("pguerville");
    QCoreApplication::setApplicationName("Kunie");

    setAttribute(Qt::AA_NativeWindows);

    initEnv();

    m_ocafApp = XCAFApp_Application::GetApplication();
    TFunction_DriverTable::Get()->AddDriver(CylinderDriver::GetID(), new CylinderDriver());
    TFunction_DriverTable::Get()->AddDriver(SphereDriver::GetID(), new SphereDriver());
    TFunction_DriverTable::Get()->AddDriver(CutDriver::GetID(), new CutDriver());

    m_window = new MainWindow(this);
    m_window->show();
}

Application::~Application()
{
    delete m_window;
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

Handle(XCAFApp_Application) Application::ocafApp()
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
    QByteArray CSF_XCAFDefaults = qgetenv("CSF_XCAFDefaults");

    if(CASROOT.isEmpty()) {
        QMessageBox::critical(NULL, applicationName(), "CASROOT not set");
        ::exit(EXIT_FAILURE);
    }

    if(CSF_PluginDefaults.isEmpty()) {
        CSF_PluginDefaults = StdResource + QDir::separator().toLatin1() + "Plugin";
        qputenv("CSF_PluginDefaults", StdResource);
    }

    if(CSF_XCAFDefaults.isEmpty()) {
        CSF_XCAFDefaults = StdResource + QDir::separator().toLatin1() + "XCAF";
        qputenv("CSF_XCAFDefaults", StdResource);
    }

    if(!QFileInfo(CSF_PluginDefaults).isReadable()) {
        QMessageBox::critical(NULL, applicationName(), CSF_PluginDefaults + " is not readable");
        ::exit(EXIT_FAILURE);
    }

    if(!QFileInfo(CSF_XCAFDefaults).isReadable()) {
        QMessageBox::critical(NULL, applicationName(), CSF_XCAFDefaults + " is not readable");
        ::exit(EXIT_FAILURE);
    }
}
