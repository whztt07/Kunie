#include "SphereDriver.h"
#include <Standard_GUID.hxx>
#include <TDataStd_Real.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <TNaming_Builder.hxx>

IMPLEMENT_STANDARD_RTTIEXT(SphereDriver, TFunction_Driver)
IMPLEMENT_STANDARD_HANDLE(SphereDriver, TFunction_Driver)

const Standard_GUID& SphereDriver::GetID()
{
    static Standard_GUID guid("5767ff68-c942-4c88-8a89-6c7a22131857");
    return guid;
}

Standard_Integer SphereDriver::Execute(TFunction_Logbook& /*log*/) const
{
    // Get the values of dimension and position attributes
    Handle(TDataStd_Real) val;
    Standard_Real x,y,z,r;

    if (!Label().FindChild(1).FindAttribute(TDataStd_Real::GetID(), val))
        return 1;
    r=val->Get();

    if (!Label().FindChild(2).FindAttribute(TDataStd_Real::GetID(), val))
        return 1;
    x=val->Get();

    if (!Label().FindChild(3).FindAttribute(TDataStd_Real::GetID(), val))
        return 1;
    y=val->Get();

    if (!Label().FindChild(4).FindAttribute(TDataStd_Real::GetID(), val))
        return 1;
    z=val->Get();

    BRepPrimAPI_MakeSphere mkSphere(gp_Pnt(x, y, z), r);
    TopoDS_Shape shape = mkSphere.Shape();

    TNaming_Builder builder(Label());
    builder.Generated(shape);

    return 0;
}
