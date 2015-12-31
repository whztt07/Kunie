#include "CylinderDriver.h"
#include <Standard_GUID.hxx>
#include <TDataStd_Real.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <TNaming_Builder.hxx>

IMPLEMENT_STANDARD_RTTIEXT(CylinderDriver, TFunction_Driver)
IMPLEMENT_STANDARD_HANDLE(CylinderDriver, TFunction_Driver)

const Standard_GUID& CylinderDriver::GetID()
{
    static Standard_GUID guid("22D22E53-D69A-11d4-8F1A-0060B0EE18E8");
    return guid;
}

Standard_Integer CylinderDriver::Execute(TFunction_Logbook& /*log*/) const
{
    Handle(TDataStd_Real) val;
    Standard_Real x,y,z,r,h;

    if (!Label().FindChild(1).FindAttribute(TDataStd_Real::GetID(), val))
        return 1;
    r=val->Get();

    if (!Label().FindChild(2).FindAttribute(TDataStd_Real::GetID(), val))
        return 1;
    h=val->Get();

    if (!Label().FindChild(3).FindAttribute(TDataStd_Real::GetID(), val))
        return 1;
    x=val->Get();

    if (!Label().FindChild(4).FindAttribute(TDataStd_Real::GetID(), val))
        return 1;
    y=val->Get();

    if (!Label().FindChild(5).FindAttribute(TDataStd_Real::GetID(), val))
        return 1;
    z=val->Get();

    BRepPrimAPI_MakeCylinder mkCylinder(gp_Ax2(gp_Pnt(x, y, z), gp_Dir(0, 0, 1)), r, h);
    TopoDS_Shape shape = mkCylinder.Shape();

    TNaming_Builder builder(Label());
    builder.Generated(shape);

    return 0;
}
