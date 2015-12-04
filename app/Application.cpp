#include "Application.h"
#include "CylinderDriver.h"
#include <TFunction_DriverTable.hxx>

Application::Application(int &argc, char **argv):
    QApplication(argc, argv)
{
    setAttribute(Qt::AA_NativeWindows);

    TFunction_DriverTable::Get()->AddDriver(CylinderDriver::GetID(), new CylinderDriver());
}

