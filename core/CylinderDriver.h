#ifndef CYLINDERDRIVER_H
#define CYLINDERDRIVER_H

#include <TFunction_Driver.hxx>

class CylinderDriver : public TFunction_Driver
{
public:
    static const Standard_GUID& GetID();
    Standard_Integer Execute(TFunction_Logbook& log) const;

    DEFINE_STANDARD_RTTI(CylinderDriver)
};

DEFINE_STANDARD_HANDLE(CylinderDriver, TFunction_Driver)

#endif // CYLINDERDRIVER_H
