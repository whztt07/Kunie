#include "CylinderDriver.h"
#include <Standard_GUID.hxx>
#include <TDataStd_Real.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <TNaming_Builder.hxx>

IMPLEMENT_STANDARD_RTTIEXT(CylinderDriver, TFunction_Driver)
IMPLEMENT_STANDARD_HANDLE(CylinderDriver, TFunction_Driver)

CylinderDriver::CylinderDriver()
{

}

const Standard_GUID &CylinderDriver::GetID()
{
    static Standard_GUID guid("22D22E53-D69A-11d4-8F1A-0060B0EE18E8");
    return guid;
}

void CylinderDriver::Validate(TFunction_Logbook& log) const
{
    TFunction_Driver::Validate(log);
}

Standard_Boolean CylinderDriver::MustExecute(const TFunction_Logbook& log) const
{
    return TFunction_Driver::MustExecute(log);
}

//=======================================================================
//function : Execute
//purpose  :
//         : We compute the object and topologically name it.
//         : If during the execution we found something wrong,
//         : we return the number of the failure. For example:
//         : 1 - an attribute hasn't been found,
//         : 2 - algorithm failed,
//         : if there are no any mistakes occurred we return 0:
//         : 0 - no mistakes were found.
//=======================================================================
Standard_Integer CylinderDriver::Execute(TFunction_Logbook& /*log*/) const
{
    // Get the values of dimension and position attributes
    Handle(TDataStd_Real) val;
    Standard_Real x,y,z,r,h;

    if (!Label().FindChild(1).FindAttribute(TDataStd_Real::GetID(), val )) return 1;
    r=val->Get();

    if (!Label().FindChild(2).FindAttribute(TDataStd_Real::GetID(), val )) return 1;
    h=val->Get();

    if (!Label().FindChild(3).FindAttribute(TDataStd_Real::GetID(), val )) return 1;
    x=val->Get();

    if (!Label().FindChild(4).FindAttribute(TDataStd_Real::GetID(), val )) return 1;
    y=val->Get();

    if (!Label().FindChild(5).FindAttribute(TDataStd_Real::GetID(), val )) return 1;
    z=val->Get();

    // Build a Cyl using the dimension and position attributes
    BRepPrimAPI_MakeCylinder mkCyl(gp_Ax2(gp_Pnt(x, y ,z), gp_Dir(0,0,1)), r, h);
    TopoDS_Shape ResultShape = mkCyl.Shape();

    // Build a TNaming_NamedShape using built Cyl
    TNaming_Builder B(Label());
    B.Generated(ResultShape);

    return 0;
}
