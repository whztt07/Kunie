#include "OcafModel.h"
#include "Document.h"
#include "LabelItem.h"

OcafModel::OcafModel(Document *doc):
    QAbstractItemModel(doc)
{
    m_rootItem = new LabelItem(doc->ocafDoc()->Main().Root());
}

OcafModel::~OcafModel()
{
    delete m_rootItem;
}

QModelIndex OcafModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    OcafItem *parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<OcafItem*>(parent.internalPointer());

    OcafItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex OcafModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    OcafItem *childItem = static_cast<OcafItem*>(index.internalPointer());
    OcafItem *parentItem = childItem->parentItem();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int OcafModel::columnCount(const QModelIndex&) const
{
    return 2;
}

int OcafModel::rowCount(const QModelIndex& parent) const
{
    OcafItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<OcafItem*>(parent.internalPointer());

    return parentItem->childCount();
}

QVariant OcafModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    OcafItem *item = static_cast<OcafItem*>(index.internalPointer());
    return item->data(index.column(), role);
}

QVariant OcafModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return "Label";
            break;
        case 1:
            return "Value";
            break;
        }
    }

    return QVariant();
}
