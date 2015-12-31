#ifndef SPHERECOMMAND_H
#define SPHERECOMMAND_H

#include <TDF_Label.hxx>

class SphereCommand
{
public:
    SphereCommand(const TDF_Label& root);

    TDF_Label createSphere(Standard_Real x, Standard_Real y, Standard_Real z,
                           Standard_Real r, const TCollection_ExtendedString& Name);

private:
    TDF_Label m_root;
};

#endif // SPHERECOMMAND_H
