#include "LabelItem.h"
#include "AttributeItem.h"
#include <TDF_Tool.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_AttributeIterator.hxx>
#include <QApplication>
#include <QStyle>

LabelItem::LabelItem(TDF_Label label, LabelItem *parent):
    OcafItem(parent),
    m_label(label)
{
    for (TDF_AttributeIterator it(m_label); it.More(); it.Next()) {
        m_children.append(new AttributeItem(it.Value(), this));
    }

    for (TDF_ChildIterator it(m_label); it.More(); it.Next()) {
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

QVariant LabelItem::data(int column, int role) const
{
    if (column == 0) {
        if (role == Qt::DisplayRole) {
            TCollection_AsciiString entry;
            TDF_Tool::Entry(m_label, entry);
            return entry.ToCString();
        } else if (role == Qt::DecorationRole) {
            return qApp->style()->standardIcon(QStyle::SP_DirIcon);
        }
    }

    return QVariant();
}
