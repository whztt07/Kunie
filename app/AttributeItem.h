#ifndef ATTRIBUTEITEM_H
#define ATTRIBUTEITEM_H

#include "OcafItem.h"

class TDF_Attribute;

class AttributeItem : public OcafItem
{
public:
    AttributeItem(TDF_Attribute* attribute, LabelItem *parent);

    OcafItem* child(int row);
    int childCount() const;
    QVariant data(int column, int role) const;

private:
    TDF_Attribute* m_attribute;
};

#endif // ATTRIBUTEITEM_H
