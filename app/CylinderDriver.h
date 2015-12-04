#ifndef CYLINDERDRIVER_H
#define CYLINDERDRIVER_H

#include <TFunction_Driver.hxx>

class CylinderDriver : public TFunction_Driver
{
public:
    CylinderDriver();

    static const Standard_GUID& GetID();
    void Validate(TFunction_Logbook& log) const;
    Standard_Boolean MustExecute(const TFunction_Logbook& log) const;
    Standard_Integer Execute(TFunction_Logbook& log) const;

    DEFINE_STANDARD_RTTI(TFunction_Driver)
};

DEFINE_STANDARD_HANDLE(CylinderDriver, TFunction_Driver)

#endif // CYLINDERDRIVER_H
