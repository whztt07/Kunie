#ifndef OCAFITEM_H
#define OCAFITEM_H

#include <QVariant>

class LabelItem;

class OcafItem
{
public:
    explicit OcafItem(LabelItem *parent = 0);
    virtual ~OcafItem();

    virtual OcafItem* child(int row) = 0;
    virtual int childCount() const = 0;
    int columnCount() const;
    virtual QVariant data(int column, int role) const = 0;
    int row() const;
    LabelItem *parentItem();

protected:
    LabelItem *m_parent;
};

#endif // OCAFITEM_H
