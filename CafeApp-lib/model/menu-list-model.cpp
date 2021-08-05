#include "menu-list-model.h"

namespace cafe {
namespace models {

MenuListModel::MenuListModel(QObject *parent, const QList<QStringList>& items)
    : QAbstractListModel (parent), items_(items)
{
}

int MenuListModel::rowCount(const QModelIndex&)const
{
    return items_.count();
}
QVariant MenuListModel::data(const QModelIndex& index, int role)const
{
    if(!index.isValid())
        return QVariant();

    if(index.row() >= items_.size())
        return QVariant();

    if(role == Qt::DisplayRole || role == Qt::EditRole)
        return items_[index.row()];
    return QVariant();
}
QVariant MenuListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();
    if(orientation == Qt::Horizontal)
        return QStringLiteral("Column %1").arg(section);

    if(orientation == Qt::Vertical)
        return QStringLiteral("Row %1").arg(section);
    return QVariant();
}

Qt::ItemFlags MenuListModel::flags(const QModelIndex& index)const
{
    if(!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}
bool MenuListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(index.isValid() && role == Qt::EditRole)
    {
        items_.replace(index.row(), QVariant(value).toStringList());
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}
bool MenuListModel::insertRows(int row, int count, const QModelIndex &)
{
    QAbstractItemModel::beginInsertRows(QModelIndex(), row, row+count-1);
    for(int n = 1; n <= count; ++n)
        items_.insert(row, {});
    QAbstractItemModel::endInsertRows();
    return true;
}
bool MenuListModel::insertRow(int row, const QModelIndex& index)
{
    return insertRows(row, 1, index);
}
bool MenuListModel::removeRows(int row, int count, const QModelIndex &)
{
    QAbstractItemModel::beginRemoveRows(QModelIndex(), row, row+count-1);
    for(int n = 0; n < count; ++n)
        items_.removeAt(row);
    QAbstractItemModel::endRemoveRows();
    return true;
}
bool MenuListModel::removeRow(int row, const QModelIndex& index)
{
    return removeRows(row, 1, index);
}
int MenuListModel::rowOf(const QString& data)const{
   if(items_.empty())
       return -1;
   for(int n = 0; n < items_.size(); ++n)
   {
       if(items_[n].indexOf(data) != -1)
           return n;
   }
   return -1;
}

}
}
