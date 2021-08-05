#include "staff-view-controller.h"
#include <QDateTime>
#include <QDebug>

namespace cafe {
namespace controllers {

class StaffViewController::Implementation
{
public:
    Implementation(StaffViewController *_staffViewController, IDBController *_dbController)
        : staffViewController(_staffViewController), dbController(_dbController)
    {
        menuModel = recieptModel();
        fModel = foodModel();
        dModel = drinkModel();
    }
    QString userID;
    models::TableModel *menuModel{ nullptr };
    models::MenuListModel *fModel{ nullptr },
                          *dModel{ nullptr };
    StaffViewController *staffViewController{ nullptr };
    IDBController *dbController{ nullptr };

    models::MenuListModel *foodModel()
    {
        auto qresult{ dbController->readFrom("SELECT items, price FROM %1 WHERE available='yes' AND itemType='Food'", "valid_menu", 2)};

        if(qresult.isEmpty())
            return new models::MenuListModel(staffViewController);
        QList<QStringList> fmdl;

        for(int i = 0; i < qresult.size(); ++i)
        {
            QStringList tmp;
            for(int j = 0; j < qresult[0].size(); ++j)
            {
                tmp << qresult[i][j].toString();
            }
            fmdl << tmp;
        }
        qDebug() << "Food Model: " << fmdl;
        return new models::MenuListModel(staffViewController, fmdl);
    }
    models::MenuListModel *drinkModel()
    {
        auto qresult{ dbController->readFrom("SELECT items, price FROM %1 WHERE available='yes' AND itemType='Drink'", "valid_menu", 2)};

        if(qresult.isEmpty())
            return new models::MenuListModel(staffViewController);
        QList<QStringList> dmdl;

        for(int i = 0; i < qresult.size(); ++i)
        {
            QStringList tmp;
            for(int j = 0; j < qresult[0].size(); ++j)
            {
                tmp << qresult[i][j].toString();
            }
            dmdl << tmp;
        }
        return new models::MenuListModel(staffViewController, dmdl);
    }
    models::TableModel *recieptModel()
    {
        //QList<QVariantList> qresult({ dbController->readFrom("SELECT items FROM %1 WHERE available='yes'", "valid_menu", 1) });

        //if(qresult.isEmpty())
        //    return nullptr;


        QList<QStringList> mdl({{"Menu", "Amount"}});

        /*for(int i = 0; i < qresult.size(); ++i)
        {
            QStringList tmp;
            for(int j = 0; j < qresult[0].size(); ++j)
            {
                tmp << qresult[i][j].toString() << "0";
            }
            mdl << tmp;
        }*/
        mdl << QStringList{"Total", "0"};
        //auto model{ new models::TableModel(staffViewController, mdl) };
        //model->sort(0);
        //return model;
        return new models::TableModel(staffViewController, mdl);
    }
    QString computeTotal(){
        int sum = 0;
        for(int n = 1, size = menuModel->rowCount()-1; n < size; ++n)
            sum += menuModel->data(menuModel->index(n, 1)).toInt();
        QString tmp{ QString::number(sum) }, total;
        if(tmp.size() > 3)
        {
            for(int n = tmp.size()-1, i = 1; n >= 0; --n, ++i)
            {
                total.prepend(tmp[n]);
                if(i % 3 == 0)
                    total.prepend(',');
            }
        }
        else return tmp;
        return total;
    }
    void addToModel(const QStringList& menu)
    {
        if(*++menu.begin() == "Food")
        {
            fModel->insertRow(fModel->rowCount());
            fModel->setData(fModel->index(fModel->rowCount()-1), QStringList{*menu.cbegin(), *--menu.cend()});
        }else
        {
            dModel->insertRow(dModel->rowCount());
            dModel->setData(dModel->index(dModel->rowCount()-1), QStringList{*menu.cbegin(), *--menu.cend()});
        }
    }
    void removeFromModel(const QStringList& menu)
    {
        if(*++menu.begin() == "Food")
        {
            fModel->removeRow(fModel->rowOf(*menu.cbegin()));
        }else
        {
            dModel->removeRow(dModel->rowOf(*menu.cbegin()));
        }
    }
    void clearTableData()
    {
        if(menuModel){
            if(menuModel->rowCount() > 2){
                menuModel->removeRows(1, menuModel->rowCount()-2);
                auto totalRep = menuModel->data(menuModel->index(menuModel->rowCount()-1, 0)).toString();
                menuModel->setData(menuModel->index(menuModel->rowCount()-1, 0), QStringList{ totalRep, "0" });
            }
        }
    }
    void addMenuToDatabase()
    {
        if(menuModel->data(menuModel->index(menuModel->rowCount()-1, 1)).toInt() == 0)
            return;
        QDateTime date_time{ QDateTime::currentDateTime() };
        QStringList items, prices, quantities;
        for(int i = 1; i < menuModel->rowCount()-1; ++i)
        {
            QString data{ menuModel->data(menuModel->index(i, 0)).toString() };
            items << data;
            prices << menuModel->data(menuModel->index(i, 1)).toString();
            if( fModel->rowOf(data) != -1 )
                quantities << "0";
            else{
                auto qresult{ dbController->readFrom("SELECT price FROM valid_menu WHERE items='%1'", data, 1) };
                if(!qresult.isEmpty())
                    quantities << QString::number(menuModel->data(menuModel->index(i, 1)).toInt() / qresult[0][0].toInt());
            }
        }
        dbController->update("INSERT INTO %1_SalesRecord VALUES(?, ?, ?, ?)", userID, { date_time.toString("d-M-yyyy:H:m:s"),
                                                                                        items.join(";"),
                                                                                        prices.join(";"),
                                                                                        quantities.join(";") });

        //clearTableData();
    }
    void addToMenu(MenuType menuType, const QString& menuName)
    {
        if(menuType == MenuType::Food)
        {
            fModel->insertRow(fModel->rowCount());
            fModel->setData(fModel->index(fModel->rowCount()-1, 0), QStringList{ menuName, "0"});
        }else
        {
            dModel->insertRow(dModel->rowCount());
            dModel->setData(dModel->index(dModel->rowCount()-1, 0), QStringList{ menuName, "0"});
        }
    }
    void removeFromMenu(MenuType menuType, int index)
    {
        auto mdlTotalIndex = menuModel->index(menuModel->rowCount()-1, 0);
        int currentTotal = menuModel->data(menuModel->index(menuModel->rowCount()-1, 1)).toInt();
        if(menuType == MenuType::Food)
        {
            int menuPrice = fModel->data(fModel->index(index, 0)).toStringList()[1].toInt();
            menuModel->setData(mdlTotalIndex, QStringList{ menuModel->data(mdlTotalIndex).toString(), QString::number(currentTotal - menuPrice) });
            fModel->removeRow(index);
        }else
        {
            int menuPurchasePrice = menuModel->data(menuModel->index(index, 1)).toInt();
            menuModel->setData(mdlTotalIndex, QStringList{ menuModel->data(mdlTotalIndex).toString(), QString::number(currentTotal - menuPurchasePrice) });
            dModel->removeRow(index);
        }
    }
    void changeMenuPrice(MenuType menuType, const QString& data, int unit)
    {
        auto mdlTotalIndex = menuModel->index(menuModel->rowCount()-1, 0);
        int currentTotal = menuModel->data(menuModel->index(menuModel->rowCount()-1, 1)).toInt();

        if(menuType == MenuType::Food)
        {
            int menuPrice = fModel->data(fModel->index(fModel->rowOf(data), 0)).toStringList()[1].toInt();
            qDebug() << "Unit: " << unit;
            menuModel->setData(mdlTotalIndex, QStringList{ menuModel->data(mdlTotalIndex).toString(), QString::number(currentTotal - menuPrice + unit) });
            fModel->setData(fModel->index(fModel->rowOf(data), 0), QStringList{ data, QString::number(unit) });
        }
        else {
            int menuActualPrice = dModel->data(dModel->index(dModel->rowOf(data), 0)).toStringList()[1].toInt(),
                menuPurchasePrice = menuModel->data(menuModel->index(menuModel->rowOf(data), 1)).toInt();
            menuModel->setData(mdlTotalIndex, QStringList{ menuModel->data(mdlTotalIndex).toString(), QString::number(currentTotal - menuPurchasePrice  + unit * menuActualPrice) });
            dModel->setData(dModel->index(dModel->rowOf(data), 0), QStringList{ data, QString::number(unit) });
        }
    }
};

StaffViewController::StaffViewController(QObject *parent, IDBController *dbController)
    : QObject(parent)
{
    implementation.reset(new Implementation(this, dbController));
    QObject::connect(this, &StaffViewController::updateDailyEntries, this, &StaffViewController::addPurchaseToDailyEntries);

}
StaffViewController::~StaffViewController()
{
}
void StaffViewController::setUserID(const QString& id)
{
    if(implementation->userID == id)
        return;
    implementation->userID = id;
    emit userIDValueChanged(id);
}
QObject *StaffViewController::foodModel()
{
    return implementation->fModel;
}

QObject *StaffViewController::drinkModel()
{
    return implementation->dModel;
}

QObject *StaffViewController::recieptModel()
{
    return implementation->menuModel;
}
QString StaffViewController::computeTotal()
{
    return implementation->computeTotal();
}
void StaffViewController::addToModel(const QStringList& addedMenu)
{
    implementation->addToModel(addedMenu);
}
void StaffViewController::removeFromModel(const QStringList& removedMenu)
{
    implementation->removeFromModel(removedMenu);
}
void StaffViewController::clearData()
{
    implementation->clearTableData();
}
void StaffViewController::addPurchaseToDailyEntries()
{
    implementation->addMenuToDatabase();
}
void StaffViewController::addToMenu(MenuType menuType, const QString& menuName)
{
    implementation->addToMenu(menuType, menuName);
}
void StaffViewController::removeMenu(MenuType menuType, int index)
{
    implementation->removeFromMenu(menuType, index);
}

void StaffViewController::changeMenuPrice(MenuType menuType, const QString& data, int newPrice)
{
    implementation->changeMenuPrice(menuType, data, newPrice);
}

}
}
