#include "LabelItem.h"
#include <TDF_Tool.hxx>
#include <TDF_ChildIterator.hxx>

LabelItem::LabelItem(TDF_Label label, LabelItem *parent):
    OcafItem(parent),
    m_label(label)
{
    cout << m_label;

    for(TDF_ChildIterator it(m_label); it.More(); it.Next()) {
        m_children.append(new LabelItem(it.Value(), this));
    }
}

LabelItem::~LabelItem()
{
    qDeleteAll(m_children);
}

OcafItem* LabelItem::child(int row)
{
    return m_children.value(row);
}

int LabelItem::childCount() const
{
    return m_children.count();
}

QVariant LabelItem::data(int column) const
{
    if(column == 0) {
        TCollection_AsciiString entry;
        TDF_Tool::Entry(m_label, entry);
        return entry.ToCString();
    }

    return QVariant();
}
