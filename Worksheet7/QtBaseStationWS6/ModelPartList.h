#ifndef VIEWER_MODELPARTLIST_H
#define VIEWER_MODELPARTLIST_H

#include "ModelPart.h"

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QString>

class ModelPartList : public QAbstractItemModel {
    Q_OBJECT
public:
    ModelPartList(const QString& data, QObject* parent = nullptr);
    ~ModelPartList();

    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    QModelIndex index(int row, int column, const QModelIndex& parent) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent) const override;

    ModelPart* getRootItem();
    QModelIndex appendChild(QModelIndex& parent, const QList<QVariant>& data);

private:
    ModelPart *rootItem = nullptr;
};

#endif
