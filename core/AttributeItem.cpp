#include "AttributeItem.h"
#include "LabelItem.h"
#include <TDF_Attribute.hxx>
#include <TDataStd_Real.hxx>
#include <TDataStd_Name.hxx>
#include <TNaming_NamedShape.hxx>
#include <QApplication>
#include <QStyle>

AttributeItem::AttributeItem(TDF_Attribute *attribute, LabelItem *parent):
    QTreeWidgetItem(parent)
{
    setData(0, Qt::DisplayRole, attribute->DynamicType()->Name());
    setIcon(0, qApp->style()->standardIcon(QStyle::SP_FileIcon));

    if (attribute->IsKind(STANDARD_TYPE(TDataStd_Real))) {
        Handle(TDataStd_Real) data = Handle(TDataStd_Real)::DownCast(attribute);
        setData(1, Qt::DisplayRole, data->Get());
    } else if (attribute->IsKind(STANDARD_TYPE(TDataStd_Name))) {
        Handle(TDataStd_Name) data = Handle(TDataStd_Name)::DownCast(attribute);
        setData(1, Qt::DisplayRole, QString::fromUtf16((const ushort*)data->Get().ToExtString()));
    } else if (attribute->IsKind(STANDARD_TYPE(TNaming_NamedShape))) {
        Handle(TNaming_NamedShape) data = Handle(TNaming_NamedShape)::DownCast(attribute);
        setData(1, Qt::DisplayRole, "Visible");
        setCheckState(1, Qt::Unchecked);
    }
}
