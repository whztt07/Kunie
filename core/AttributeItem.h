#ifndef ATTRIBUTEITEM_H
#define ATTRIBUTEITEM_H

#include <QTreeWidgetItem>

class LabelItem;
class TDF_Attribute;

class AttributeItem : public QTreeWidgetItem
{
public:
    AttributeItem(TDF_Attribute* attribute, LabelItem *parent);
};

#endif // ATTRIBUTEITEM_H
