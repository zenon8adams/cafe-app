#ifndef MENULISTMODEL_H
#define MENULISTMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QList>
#include <QPair>
#include <QModelIndex>
#include <cafeapp-lib_global.h>
namespace cafe {
namespace models {

class CAFEAPPLIBSHARED_EXPORT MenuListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    MenuListModel(QObject *parent = nullptr, const QList<QStringList>& items = {});
    ~MenuListModel() override = default;
    int rowCount(const QModelIndex& index = QModelIndex())const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole)const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index = QModelIndex())const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    bool insertRows(int row, int count, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &index = QModelIndex()) override;
    Q_INVOKABLE bool insertRow(int row, const QModelIndex& index = QModelIndex());
    Q_INVOKABLE bool removeRow(int row, const QModelIndex &index = QModelIndex());
    Q_INVOKABLE int rowOf(const QString& data)const;
private:
    QList<QStringList> items_;
};

}
}

Q_DECLARE_METATYPE(cafe::models::MenuListModel*)

#endif // MENULISTMODEL_H
