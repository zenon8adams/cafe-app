#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QList>
#include <QStringList>
#include <cafeapp-lib_global.h>

namespace cafe {
namespace models {

class CAFEAPPLIBSHARED_EXPORT TableModel : public QAbstractTableModel
{
    Q_OBJECT
    enum TableRoles{
        HeadingRole = Qt::UserRole + 1,
        FooterRole
    };
public:
    explicit TableModel(QObject *parent = nullptr, const QList<QStringList>& table = {});

    Q_INVOKABLE int rowCount(const QModelIndex & = QModelIndex()) const override;
    Q_INVOKABLE int columnCount(const QModelIndex & = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    bool insertRows(int row, int count, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &index = QModelIndex()) override;
    Q_INVOKABLE bool insertRow(int row, const QModelIndex& index = QModelIndex());
    Q_INVOKABLE bool removeRow(int row, const QModelIndex &index = QModelIndex());
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE int rowOf(const QString& data)const;
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
    void sortAll(Qt::SortOrder order = Qt::AscendingOrder);

signals:

public slots:
private:
    QList<QStringList> table;
};

}
}

Q_DECLARE_METATYPE(cafe::models::TableModel*)

#endif // TABLEMODEL_H
