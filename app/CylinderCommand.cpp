#include "CylinderCommand.h"
#include "CylinderDriver.h"

#include <QDebug>

#include <TDF_TagSource.hxx>
#include <TDataStd_Real.hxx>
#include <TDataStd_Name.hxx>
#include <TFunction_Logbook.hxx>
#include <TFunction_Function.hxx>
#include <TFunction_DriverTable.hxx>

CylinderCommand::CylinderCommand(const TDF_Label& root):
    m_root(root)
{
}

TDF_Label CylinderCommand::createCylinder(Standard_Real x, Standard_Real y, Standard_Real z,
                                          Standard_Real r, Standard_Real h, const TCollection_ExtendedString& Name)
{
    // A data structure for our cylinder:
    // the cylinder itself is attached to the CylinderLabel label (as his name and his function attribute)
    // its arguments (dimensions: radius and height; and position: x, y, z)
    // are attached to the child labels of the cylinder:
    //
    // 0:1 Cylinder Label ---> Name --->  Named shape ---> Function
    //       |
    //     0:1:1 -- Radius Label
    //       |
    //     0:1:2 -- Height Label
    //       |
    //     0:1:3 -- X Label
    //       |
    //     0:1:4 -- Y Label
    //       |
    //     0:1:5 -- Z Label

    // Create a new label in the data structure for the cylinder
    TDF_Label label = TDF_TagSource::NewChild(m_root);

    // Create the data structure : Set the dimensions, position and name attributes
    TDataStd_Real::Set(label.FindChild(1), r);
    TDataStd_Real::Set(label.FindChild(2), h);
    TDataStd_Real::Set(label.FindChild(3), x);
    TDataStd_Real::Set(label.FindChild(4), y);
    TDataStd_Real::Set(label.FindChild(5), z);
    TDataStd_Name::Set(label, Name);


    // Instanciate a TFunction_Function attribute connected to the current cylinder driver
    // and attach it to the data structure as an attribute of the Cylinder Label
    TFunction_Function::Set(label, CylinderDriver::GetID());

    // Initialize and execute the cylinder driver (look at the "Execute()" code)
    TFunction_Logbook log;

    Handle(CylinderDriver) driver;
    // Find the TOcafFunction_CylDriver in the TFunction_DriverTable using its GUID
    if(!TFunction_DriverTable::Get()->FindDriver(CylinderDriver::GetID(), driver)) return label;

    driver->Init(label);
    if(driver->Execute(log))
        qCritical() << "Function_Execute : failed" << "Cylinder";

    return label;
}
