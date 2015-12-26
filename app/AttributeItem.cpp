#include "AttributeItem.h"
#include <TDF_Attribute.hxx>
#include <TDataStd_Real.hxx>
#include <TDataStd_Name.hxx>
#include <QApplication>
#include <QStyle>

AttributeItem::AttributeItem(TDF_Attribute *attribute, LabelItem *parent):
    OcafItem(parent),
    m_attribute(attribute)
{

}

OcafItem* AttributeItem::child(int)
{
    return NULL;
}

int AttributeItem::childCount() const
{
    return 0;
}

QVariant AttributeItem::data(int column, int role) const
{
    if (column == 0) {
        if (role == Qt::DisplayRole) {
            return m_attribute->DynamicType()->Name();
        } else if (role == Qt::DecorationRole) {
            return qApp->style()->standardIcon(QStyle::SP_FileIcon);
        }
    }

    if (column == 1 && role == Qt::DisplayRole) {
        if (m_attribute->IsKind(STANDARD_TYPE(TDataStd_Real))) {
            Handle(TDataStd_Real) attribute = Handle(TDataStd_Real)::DownCast(m_attribute);
            return attribute->Get();
        } else if (m_attribute->IsKind(STANDARD_TYPE(TDataStd_Name))) {
            Handle(TDataStd_Name) attribute = Handle(TDataStd_Name)::DownCast(m_attribute);
            return QString::fromUtf16((const ushort*)attribute->Get().ToExtString());
        }
    }

    return QVariant();
}
