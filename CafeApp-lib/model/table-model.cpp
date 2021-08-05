#include "table-model.h"
#include <QDebug>

namespace cafe {
namespace models {

TableModel::TableModel(QObject *parent, const QList<QStringList> &_table)
    : QAbstractTableModel(parent), table(_table)
{

}

int TableModel::rowCount(const QModelIndex &) const
{
    return table.size();//Number of rows
}

int TableModel::columnCount(const QModelIndex &) const
{
    return table.at(0).size();//Columns
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
    {
        return  table.at(index.row()).at(index.column());
    }
    case HeadingRole:
    {
        return index.row()==0;
    }
    case FooterRole:
    {
        return index.row() == table.size()-1;
    }
    default:
        break;
    }

    return QVariant();
}
QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != HeadingRole)
        return QVariant();
    if(orientation == Qt::Horizontal)
        return QStringLiteral("Column %1").arg(section);

    if(orientation == Qt::Vertical)
        return QStringLiteral("Row %1").arg(section);
    return QVariant();
}
bool TableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(index.isValid() && role == Qt::EditRole)
    {
        table.replace(index.row(), QVariant(value).toStringList());
        sort(0);
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}
bool TableModel::insertRows(int row, int count, const QModelIndex &)
{
    QAbstractItemModel::beginInsertRows(QModelIndex(), row, row+count-1);
    for(int n = 1; n <= count; ++n)
        table.insert(row, {});
    QAbstractItemModel::endInsertRows();
    return true;
}
bool TableModel::insertRow(int row, const QModelIndex& index)
{
    return insertRows(row, 1, index);
}
bool TableModel::removeRows(int row, int count, const QModelIndex &)
{
    QAbstractItemModel::beginRemoveRows(QModelIndex(), row, row+count-1);
    for(int n = 0; n < count; ++n)
        table.removeAt(row);
    QAbstractItemModel::endRemoveRows();
    return true;
}
bool TableModel::removeRow(int row, const QModelIndex& index)
{
    return removeRows(row, 1, index);
}
int TableModel::rowOf(const QString& data)const{
   if(table.empty())
       return -1;
   for(int n = 0; n < table.size(); ++n)
   {
       if(table[n].indexOf(data) != -1)
           return n;
   }
   return -1;
}
QHash<int, QByteArray> TableModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "tabledata";
    roles[HeadingRole] = "heading";
    roles[FooterRole] = "footer";
    return roles;
}
void TableModel::sort(int column, Qt::SortOrder order)
{
    if(order == Qt::AscendingOrder)
        std::sort(++table.begin(), --table.end(), [column](const auto& lhs, const auto& rhs){ return lhs[column] < rhs[column]; });
    else
        std::sort(++table.begin(), --table.end(), [column](const auto& lhs, const auto& rhs){ return lhs[column] > rhs[column]; });
}
void TableModel::sortAll(Qt::SortOrder order)
{
    for(int column = 0; column < columnCount(); ++column){
        if(order == Qt::AscendingOrder)
            std::sort(++table.begin(), --table.end(), [column](const auto& lhs, const auto& rhs){ return lhs[column] < rhs[column]; });
        else
            std::sort(++table.begin(), --table.end(), [column](const auto& lhs, const auto& rhs){ return lhs[column] > rhs[column]; });
    }

}
}
}
