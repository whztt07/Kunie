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

    virtual OcafItem* child(int row);
    virtual int childCount() const;
    virtual QVariant data(int column) const;
    int row() const;

private:
    TDF_Label m_label;
    QList<OcafItem*> m_children;

    friend class OcafItem;
};

#endif // LABELITEM_H
