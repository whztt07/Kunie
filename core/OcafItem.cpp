#include "OcafItem.h"
#include "LabelItem.h"

OcafItem::OcafItem(LabelItem *parent):
    m_parent(parent)
{

}

OcafItem::~OcafItem()
{

}

int OcafItem::columnCount() const
{
    return 2;
}

Qt::ItemFlags OcafItem::flags() const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

int OcafItem::row() const
{
    if (m_parent)
        return m_parent->m_children.indexOf(const_cast<OcafItem*>(this));

    return 0;
}

LabelItem *OcafItem::parentItem()
{
    return m_parent;
}
