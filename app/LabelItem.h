#ifndef LABELITEM_H
#define LABELITEM_H

#include "OcafItem.h"
#include <TDF_Label.hxx>
#include <QList>

class LabelItem : public OcafItem
{
public:
    LabelItem(TDF_Label label, LabelItem *parent = 0);
    ~LabelItem();

    OcafItem* child(int row);
    int childCount() const;
    QVariant data(int column, int role) const;

private:
    TDF_Label m_label;
    QList<OcafItem*> m_children;

    friend class OcafItem;
};

#endif // LABELITEM_H
