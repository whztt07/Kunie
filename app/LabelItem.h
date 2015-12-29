#ifndef LABELITEM_H
#define LABELITEM_H

#include <QTreeWidgetItem>
#include <TDF_Label.hxx>

class LabelItem : public QTreeWidgetItem
{
public:
    LabelItem(TDF_Label label, LabelItem* parent = 0);
    ~LabelItem();
};

#endif // LABELITEM_H
