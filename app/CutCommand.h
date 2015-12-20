#ifndef CUTCOMMAND_H
#define CUTCOMMAND_H

#include <TDF_Label.hxx>

class CutCommand
{
public:
    CutCommand(const TDF_Label& root);

    TDF_Label createCut(const TDF_Label& baseLab, const TDF_Label& toolLab, const TCollection_ExtendedString& name);

private:
    TDF_Label m_root;
};

#endif // CUTCOMMAND_H
