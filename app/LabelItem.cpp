#include "LabelItem.h"
#include "AttributeItem.h"
#include <TDF_Tool.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_AttributeIterator.hxx>
#include <QApplication>
#include <QStyle>

LabelItem::LabelItem(TDF_Label label, LabelItem *parent):
    QTreeWidgetItem(parent)
{
    TCollection_AsciiString entry;
    TDF_Tool::Entry(label, entry);
    setData(0, Qt::DisplayRole, entry.ToCString());
    setIcon(0, qApp->style()->standardIcon(QStyle::SP_DirIcon));

    for (TDF_AttributeIterator it(label); it.More(); it.Next()) {
        addChild(new AttributeItem(it.Value(), this));
    }

    for (TDF_ChildIterator it(label); it.More(); it.Next()) {
        addChild(new LabelItem(it.Value(), this));
    }
}

LabelItem::~LabelItem()
{
    qDeleteAll(takeChildren());
}
