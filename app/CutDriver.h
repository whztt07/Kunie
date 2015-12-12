#ifndef CUTDRIVER_H
#define CUTDRIVER_H

#include <TFunction_Driver.hxx>

class CutDriver : public TFunction_Driver
{
public:
    static const Standard_GUID& GetID();
    Standard_Integer Execute(TFunction_Logbook& log) const;

    DEFINE_STANDARD_RTTI(CutDriver)
};

DEFINE_STANDARD_HANDLE(CutDriver, TFunction_Driver)


#endif // CUTDRIVER_H
