#include "CutCommand.h"
#include "CutDriver.h"

#include <QDebug>

#include <TDF_TagSource.hxx>
#include <TDF_Reference.hxx>
#include <TDataStd_Name.hxx>
#include <TFunction_Logbook.hxx>
#include <TFunction_Function.hxx>
#include <TFunction_DriverTable.hxx>

CutCommand::CutCommand(const TDF_Label& root):
    m_root(root)
{
}

TDF_Label CutCommand::createCut(const TDF_Label& baseLab, const TDF_Label& toolLab, const TCollection_ExtendedString& name)
{
    // A data structure for our cut operation:
    // the result itself is attached to the Result label (as his name and his function attribute)
    // its arguments (Original object label; Tool object label) are attached to the child
    // labels of the Result Label:
    //
    // 0:1 Result Label ---> Name --->  Named shape ---> Function
    //       |
    //     0:1:1 -- Original object label Label
    //       |
    //     0:1:2 --  object label Label
    //

    // Create a new label in the data structure for the box
    TDF_Label label = TDF_TagSource::NewChild(m_root);

    // Create the data structure : Set a reference attribute on the Original and the Tool objects, set the name attribute
    TDF_Reference::Set(label.FindChild(1), baseLab);
    TDF_Reference::Set(label.FindChild(2), toolLab);
    TDataStd_Name::Set(label, name);

    // Instanciate a TFunction_Function attribute connected to the current cut driver
    // and attach it to the data structure as an attribute of the Result Label
    TFunction_Function::Set(label, CutDriver::GetID());

    // Initialize and execute the cut driver (look at the "Execute()" code)
    TFunction_Logbook log;

    Handle(CutDriver) driver;
    // Find the TOcafFunction_CutDriver in the TFunction_DriverTable using its GUID
    if(!TFunction_DriverTable::Get()->FindDriver(CutDriver::GetID(), driver)) return label;

    driver->Init(label);
    if (driver->Execute(log))
        qCritical() << "Function_Execute : failed" << "Cut";

    return label;
}
