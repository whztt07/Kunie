#ifndef OCAFMODEL_H
#define OCAFMODEL_H

#include <QAbstractItemModel>

class Document;
class OcafItem;

class OcafModel : public QAbstractItemModel
{
public:
    OcafModel(Document* doc);
    ~OcafModel();

    QModelIndex	index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    QModelIndex	parent(const QModelIndex& index) const;

    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    int	rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex & index) const;

private:
    OcafItem* m_rootItem;
};

#endif // OCAFMODEL_H
