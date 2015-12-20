#ifndef CYLINDERCOMMAND_H
#define CYLINDERCOMMAND_H

#include <TDF_Label.hxx>

class CylinderCommand
{
public:
    CylinderCommand(const TDF_Label& root);

    TDF_Label createCylinder(Standard_Real x, Standard_Real y, Standard_Real z,
                             Standard_Real r, Standard_Real h, const TCollection_ExtendedString& name);

private:
    TDF_Label m_root;
};

#endif // CYLINDERCOMMAND_H
