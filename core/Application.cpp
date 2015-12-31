#include "Application.h"
#include "Document.h"
#include "MainWindow.h"
#include "CylinderDriver.h"
#include "SphereDriver.h"
#include "CutDriver.h"
#include <XCAFApp_Application.hxx>
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

    Q_INIT_RESOURCE(resources);

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
    foreach (Document* doc, m_documents) {
        m_ocafApp->Close(doc->ocafDoc());
        delete doc;
    }
    delete m_window;
}

MainWindow *Application::window()
{
    return m_window;
}

Document* Application::newDoc()
{
    Handle(TDocStd_Document) ocafDoc;
    m_ocafApp->NewDocument("MDTV-XCAF", ocafDoc);
    m_documents.append(new Document(ocafDoc));
    return m_documents.last();
}

Document* Application::openDoc(const QString& file)
{
    Handle(TDocStd_Document) ocafDoc;

    int index = m_ocafApp->IsInSession(file.toUtf8().data());

    if (index) {
        m_ocafApp->GetDocument(index, ocafDoc);
        foreach (Document* doc, m_documents) {
            if (doc->ocafDoc() == ocafDoc) {
                return doc;
            }
        }
    }

    PCDM_ReaderStatus status = m_ocafApp->Open(file.toUtf8().data(), ocafDoc);

    if (status != PCDM_RS_OK) {
        emit error(QFileInfo(file).fileName() + " not open");
        return NULL;
    }

    m_documents.append(new Document(ocafDoc));
    return m_documents.last();
}

void Application::closeDoc(Document *doc)
{
    if(m_documents.removeOne(doc)) {
        m_ocafApp->Close(doc->ocafDoc());
        delete doc;
    }
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
