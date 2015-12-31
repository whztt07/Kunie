#include "CutDriver.h"
#include <Standard_GUID.hxx>
#include <TDF_Reference.hxx>
#include <TNaming_NamedShape.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <TNaming_Builder.hxx>
#include <QDebug>

IMPLEMENT_STANDARD_RTTIEXT(CutDriver, TFunction_Driver)
IMPLEMENT_STANDARD_HANDLE(CutDriver, TFunction_Driver)

const Standard_GUID& CutDriver::GetID()
{
    static Standard_GUID guid("7485cdcf-fab1-481e-987d-cca0ea4c40d0");
    return guid;
}

Standard_Integer CutDriver::Execute(TFunction_Logbook& /*log*/) const
{
    Handle(TDF_Reference)  baseRef, toolRef;

    if (!Label().FindChild(1).FindAttribute(TDF_Reference::GetID(), baseRef)) return 1;
    TDF_Label baseLab = baseRef->Get();

    if (!Label().FindChild(2).FindAttribute(TDF_Reference::GetID(), toolRef)) return 1;
    TDF_Label toolLab = toolRef->Get();

    Handle(TNaming_NamedShape) baseNShape, toolNShape;

    if (!(baseLab.FindAttribute(TNaming_NamedShape::GetID(), baseNShape))) return 1;
    if (!(toolLab.FindAttribute(TNaming_NamedShape::GetID(), toolNShape))) return 1;

    TopoDS_Shape baseShape  = baseNShape->Get();
    TopoDS_Shape toolShape = toolNShape->Get();

    BRepAlgoAPI_Cut mkCut(baseShape, toolShape);

    // Let's check if the Cut has been successfull:
    if (!mkCut.IsDone()) {
        qCritical() << "Cut not done." << "Cut Function Driver";
        return 1;
    }

    TopoDS_Shape cutShape = mkCut.Shape();

    TNaming_Builder builder(Label());
    builder.Modify(baseShape, cutShape);

    return 0;
}
