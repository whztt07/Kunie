#ifndef SPHEREDRIVER_H
#define SPHEREDRIVER_H

#include <TFunction_Driver.hxx>

class SphereDriver : public TFunction_Driver
{
public:
    static const Standard_GUID& GetID();
    Standard_Integer Execute(TFunction_Logbook& log) const;

    DEFINE_STANDARD_RTTI(SphereDriver)
};

DEFINE_STANDARD_HANDLE(SphereDriver, TFunction_Driver)

#endif // SPHEREDRIVER_H
