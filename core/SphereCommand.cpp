#include "SphereCommand.h"
#include "SphereDriver.h"

#include <QDebug>

#include <TDF_TagSource.hxx>
#include <TDataStd_Real.hxx>
#include <TDataStd_Name.hxx>
#include <TFunction_Logbook.hxx>
#include <TFunction_Function.hxx>
#include <TFunction_DriverTable.hxx>

SphereCommand::SphereCommand(const TDF_Label& root):
    m_root(root)
{
}

TDF_Label SphereCommand::createSphere(Standard_Real x, Standard_Real y, Standard_Real z,
                                      Standard_Real r, const TCollection_ExtendedString& Name)
{
    // A data structure for our Sphere:
    // the Sphere itself is attached to the SphereLabel label (as his name and his function attribute)
    // its arguments (dimensions: radius and height; and position: x, y, z)
    // are attached to the child labels of the Sphere:
    //
    // 0:1 Sphere Label ---> Name --->  Named shape ---> Function
    //       |
    //     0:1:1 -- Radius Label
    //       |
    //     0:1:3 -- X Label
    //       |
    //     0:1:4 -- Y Label
    //       |
    //     0:1:5 -- Z Label

    // Create a new label in the data structure for the Sphere
    TDF_Label label = TDF_TagSource::NewChild(m_root);

    // Create the data structure : Set the dimensions, position and name attributes
    TDataStd_Real::Set(label.FindChild(1), r);
    TDataStd_Real::Set(label.FindChild(2), x);
    TDataStd_Real::Set(label.FindChild(3), y);
    TDataStd_Real::Set(label.FindChild(4), z);
    TDataStd_Name::Set(label, Name);

    // Instanciate a TFunction_Function attribute connected to the current Sphere driver
    // and attach it to the data structure as an attribute of the Sphere Label
    TFunction_Function::Set(label, SphereDriver::GetID());

    // Initialize and execute the Sphere driver (look at the "Execute()" code)
    TFunction_Logbook log;

    Handle(SphereDriver) driver;
    // Find the TOcafFunction_CylDriver in the TFunction_DriverTable using its GUID
    if(!TFunction_DriverTable::Get()->FindDriver(SphereDriver::GetID(), driver)) return label;

    driver->Init(label);
    if(driver->Execute(log))
        qCritical() << "Function_Execute : failed" << "Sphere";

    return label;
}
