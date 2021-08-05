#include "master-view-controller.h"
#include <QDateTime>
#include <tuple>
#include <QDebug>
#include <QStringListModel>
#include <QVector>

namespace cafe {
namespace controllers {

class MasterViewController::Implementation
{
public:
    Implementation(MasterViewController *_masterViewController, IDBController *_dbController)
        : masterViewController(_masterViewController), dbController(_dbController)
    {

        menuFormController = new MenuFormController(masterViewController);
        toolTipDescriptionController = new ToolTipDescriptionController(masterViewController, dbController);
        staffModel = new QStringListModel(masterViewController);
        foodModel = new QStringListModel(masterViewController);
        drinkModel = new QStringListModel(masterViewController);
        staffsOnDutyModel = new QStringListModel(masterViewController);
        updateModels();
    }
    MasterViewController *masterViewController{ nullptr };
    IDBController* dbController{ nullptr };
    MenuFormController *menuFormController{ nullptr };
    ToolTipDescriptionController *toolTipDescriptionController{ nullptr };
    QStringListModel *staffModel{ nullptr },
                     *foodModel{ nullptr },
                     *drinkModel{ nullptr },
                     *staffsOnDutyModel{ nullptr };
    QStringList staffs_id, staffsOnDuty;

    QPair<QString, QString> weekDateRange, yearDateRange;

    void updateModels()
    {
        staffs_id.clear();
        staffModel->removeRows(0, staffModel->rowCount());
        foodModel->removeRows(0, foodModel->rowCount());
        drinkModel->removeRows(0, drinkModel->rowCount());

        auto qresult = dbController->readFrom("SELECT id, first_name, last_name FROM %1 WHERE userType='Staff'", "valid_users", 3);
        qDebug() << "QResult: " << qresult;
        if(!qresult.isEmpty())
        {
            int staffRow = 0;
           for(const auto& elm : qresult)
           {
               staffs_id << elm.begin()->toString();
               //if((++elm.begin())->toString() == "active")
               //{
               QStringList rem;
               for(auto begin = elm.crbegin(), end = --elm.crend(); begin != end; ++begin)
                   rem << begin->toString();
               staffModel->insertRow(staffRow);
               staffModel->setData(staffModel->index(staffRow++), rem.join(" "));
               //}
               qDebug() << "Staff Full-name: " << rem.join(" ");
           }
        }
        qresult = dbController->readFrom("SELECT items, itemType FROM %1 WHERE available='yes'", "valid_menu", 2);
        if(!qresult.isEmpty())
        {
            int foodRow = 0, drinkRow = 0;
           for(const auto& elm : qresult)
           {
               if((--elm.end())->toString() == "Food")
               {
                   foodModel->insertRow(foodRow);
                   foodModel->setData(foodModel->index(foodRow++), elm.begin()->toString());
               }else
               {
                   drinkModel->insertRow(drinkRow);
                   drinkModel->setData(drinkModel->index(drinkRow++), elm.begin()->toString());
               }
           }
        }
    }
    void updateStaffsOnDuty()
    {
        QDate date{ QDate::currentDate() };
        for(const auto& staff_id : staffs_id)
        {
            auto qresult{ dbController->readFrom("SELECT date_time FROM " + staff_id + "_SalesRecord WHERE date_time LIKE '%1%'", date.toString("d-M-yyyy"), 1) };
            if(!qresult.isEmpty())
            {
                int row = staffs_id.indexOf(staff_id);
                QString tmp{ staffModel->data(staffModel->index(row, 0)).toString() };
                if(!tmp.isEmpty() && staffsOnDuty.indexOf(tmp) == -1)
                {
                    staffsOnDuty << tmp;
                    staffsOnDutyModel->insertRow(staffsOnDutyModel->rowCount());
                    staffsOnDutyModel->setData(staffsOnDutyModel->index(staffsOnDutyModel->rowCount()-1, 0), *--staffsOnDuty.end());
                }
            }
        }
    }
    bool addToDatabase()
    {
        auto qresult{ dbController->readFrom("SELECT items FROM valid_menu WHERE items='%1' AND available='yes'", menuFormController->menuName(), 1) };
        if(!qresult.isEmpty())
            return false;
        qresult = dbController->readFrom("SELECT itemType FROM valid_menu WHERE items='%1'", menuFormController->menuName(), 1);
        QString menuType = menuFormController->menuType() == MenuFormController::MenuType::Food ? "Food" : "Drink",
                nextIndex("1");
        auto updateMenu = [this, menuType]()
        {
            if(menuType == "Food")
            {
                foodModel->insertRow(foodModel->rowCount());
                foodModel->setData(foodModel->index(foodModel->rowCount()-1), menuFormController->menuName());
            }else
            {
                drinkModel->insertRow(drinkModel->rowCount());
                drinkModel->setData(drinkModel->index(drinkModel->rowCount()-1), menuFormController->menuName());
            }
        };
        if(!qresult.isEmpty()){
            qDebug() << "Qresult-exec";
            updateMenu();
            return dbController->update("UPDATE valid_menu SET available='yes' WHERE items='%1'", menuFormController->menuName(), {});
        }
        bool added = false;
        qresult = dbController->readFrom("SELECT ROWID FROM %1 ORDER BY id", "valid_menu", 1);  // Need fix!
        if(!qresult.isEmpty())
        {
            int i = 1;
            for(; i <= qresult.size() && qresult[i-1][0].toInt() == i; ++i);
            nextIndex = (i == qresult.size()) ? QString::number(i+1) : QString::number(i);
        }
        auto t_menuPrice = menuFormController->menuPrice();
        if(t_menuPrice.isEmpty())
            t_menuPrice.append('0');
        added = dbController->update("INSERT INTO %1 VALUES(?, ?, ?, ?, ?, ?, ?)", "valid_menu", {
                                                                               nextIndex,
                                                                               menuFormController->menuName(),
                                                                               menuType,
                                                                               t_menuPrice,
                                                                               "yes",
                                                                               QDate::currentDate().toString("dd-MM-yyyy"),
                                                                               ""
                                                                              });
        updateMenu();


        return added;
    }
    QStringListModel *staffs()
    {
        return staffModel;
    }
    QStringListModel *foods()
    {
        return  foodModel;
    }
    QStringListModel *drinks()
    {
        return drinkModel;
    }
    bool removeMenu(MenuType menuType, int index)
    {
        bool removed = false;
        QString data;
        if(menuType == MenuType::Food)
        {
            data = foodModel->data(foodModel->index(index)).toString();
            removed = dbController->deleteFrom("UPDATE valid_menu SET available='no', removedOn='" + QDate::currentDate().toString("dd-MM-yyyy") + "' WHERE items='%1'", data);
            foodModel->removeRow(index);
            menuFormController->setMenuName(data);
            menuFormController->setMenuType(MenuFormController::Food);
        }else{
            data = drinkModel->data(drinkModel->index(index)).toString();
            removed = dbController->deleteFrom("UPDATE valid_menu SET available='no', removedOn='" + QDate::currentDate().toString("dd-MM-yyyy") + "' WHERE items='%1'", data);
            drinkModel->removeRow(index);
            menuFormController->setMenuName(data);
            menuFormController->setMenuType(MenuFormController::Drink);
        }
        return removed;
    }
    bool isStaffActive(int index)const
    {
        auto qresult{ dbController->readFrom("SELECT state FROM valid_users WHERE id='%1'", staffs_id[index], 1) };
        if(!qresult.isEmpty())
        {
            if(qresult[0][0].toString() == "sacked")
                return false;
            return true;
        }
        return false;
    }
    bool sackStaff(int index, const QString& reason = "")
    {
        return dbController->update("UPDATE valid_users SET state='sacked', leftOn='" + QDate::currentDate().toString("dd-MM-yyyy") + "', ReasonForLeaving='" + reason + "' WHERE id='%1'", staffs_id[index], {});
    }
    void updateStaff(const QString& staffName)
    {
        staffModel->insertRow(staffModel->rowCount());
        staffModel->setData(staffModel->index(staffModel->rowCount()-1), staffName);
        updateModels();
        updateStaffsOnDuty();
    }
    QStringListModel *staffsOnDutyOn(const QString& date)const
    {
        QStringList parsedDate{ date.split("-") }, staffsOnDuty;
        QString row_id;
        if(auto qDate = QDate(parsedDate[2].toInt(), parsedDate[1].toInt(), parsedDate[0].toInt()); qDate.isValid())
            row_id = qDate.toString("d-M-yyyy");
        else return {};
        auto qresult{ dbController->readFrom("SELECT staffsOnDuty FROM dailyEntries WHERE date='%1'", row_id, 1) };
        if(!qresult.isEmpty())
            staffsOnDuty = qresult[0][0].toString().split(";");
        return new QStringListModel(staffsOnDuty, masterViewController);
    }
    QList<QObject *> summaryOn(const QString& date)
    {
        QList<QObject *> staff_entries;
        QStringList parsedDate{ date.split("-") };
        QString row_id;
        if(auto qDate = QDate(parsedDate[2].toInt(), parsedDate[1].toInt(), parsedDate[0].toInt()); qDate.isValid())
            row_id = qDate.toString("d-M-yyyy");
        else return {};
        auto qresult{ dbController->readFrom("SELECT items FROM %1", "valid_menu", 1) };
        QStringList table_header("Time"), staffs_id, staffsOnDuty, sTotalSales;
        for(int n = 0; n < qresult.size(); ++n)
            table_header << qresult[n][0].toString();
        table_header << "Total";
        qresult = dbController->readFrom("SELECT id FROM %1 WHERE userType='Staff'", "valid_users", 1);
        if(!qresult.isEmpty())
        {
            for(const auto& elm : qresult)
                staffs_id << elm.begin()->toString();
        }
        for(auto& elm : staffs_id)
        {
            qresult = dbController->readFrom("SELECT date_time FROM " + elm + "_SalesRecord WHERE date_time LIKE '%1%'", row_id, 1);

            if(qresult.empty())
                continue;
            int total_sales = 0;
            QVector<QVector<QString>> entries(qresult.size() + 1);
            entries[0] = QVector<QString>::fromList(table_header);
            int idx = 1;
            for(auto begin = qresult.cbegin(), end = qresult.cend(); begin != end; ++begin)
            {
                entries[idx].resize(table_header.size());
                entries[idx++][0] = QDateTime::fromString((*begin)[0].toString(), "d-M-yyyy:H:m:s").toString("H:m");
            }
            auto data = dbController->readFrom("SELECT item_names, prices, quantities FROM " + elm + "_SalesRecord WHERE date_time LIKE '%1%'", row_id, 3);
            std::tuple<QStringList, QStringList, QStringList> data_entry;
            idx = 1;
            for(auto begin = data.cbegin(), end = data.cend(); begin != end; ++begin, ++idx)
            {
                QString items_stream{ (*begin)[0].toString() },
                        prices_stream{ (*begin)[1].toString() },
                        quantites_stream{ (*begin)[2].toString() };
                std::get<0>(data_entry) = items_stream.split(";", QString::SkipEmptyParts);
                std::get<1>(data_entry) = prices_stream.split(";", QString::SkipEmptyParts);
                std::get<2>(data_entry) = quantites_stream.split(";", QString::SkipEmptyParts);
               int sum = 0;
                for(int n = 0, size = std::get<0>(data_entry).size(); n < size; ++n)
                {
                    entries[idx][ entries[0].indexOf(std::get<0>(data_entry)[n]) ] = std::get<1>(data_entry)[n];
                    sum += std::get<1>(data_entry)[n].toInt();
                }
                entries[idx][table_header.size()-1] = QString::number(sum);
                total_sales += sum;
            }
            entries.insert(entries.end(), QVector<QString>(table_header.size()));
            entries[entries.size()-1][table_header.size()-1] = QString::number(total_sales);
            sTotalSales << QString::number(total_sales);
            QList<QStringList> lentries;
            for(const auto& elm : entries)
                lentries << elm.toList();
            staff_entries.insert(staff_entries.end(), new models::TableModel(masterViewController, lentries));
        }
        return staff_entries;
    }
    QList<QObject *> dailySummary()
    {
        updateStaffsOnDuty();
        qDebug() << "staffs_id: " << staffs_id << "staffsOnDuty: " << staffsOnDuty;

        QList<QObject *> staff_entries;

        QString row_id{ QDate::currentDate().toString("d-M-yyyy") };


        auto qresult{ dbController->readFrom("SELECT items FROM %1", "valid_menu", 1) };
        QStringList table_header("Time");
        for(int n = 0; n < qresult.size(); ++n)
            table_header << qresult[n][0].toString();
        table_header << "Total";
        QStringList sTotalSales;
        for(auto& elm : staffs_id)
        {
            qresult = dbController->readFrom("SELECT date_time FROM " + elm + "_SalesRecord WHERE date_time LIKE '%1%'", row_id, 1);

            if(qresult.empty())
                continue;
            int total_sales = 0;
            QVector<QVector<QString>> entries(qresult.size() + 1);
            entries[0] = QVector<QString>::fromList(table_header);
            int idx = 1;
            for(auto begin = qresult.cbegin(), end = qresult.cend(); begin != end; ++begin){
                entries[idx].resize(table_header.size());
                entries[idx++][0] = QDateTime::fromString((*begin)[0].toString(), "d-M-yyyy:H:m:s").toString("H:m");
            }
            auto data = dbController->readFrom("SELECT item_names, prices, quantities FROM " + elm + "_SalesRecord WHERE date_time LIKE '%1%'", row_id, 3);
            std::tuple<QStringList, QStringList, QStringList> data_entry;
            idx = 1;
            for(auto begin = data.cbegin(), end = data.cend(); begin != end; ++begin, ++idx)
            {
                QString items_stream{ (*begin)[0].toString() },
                        prices_stream{ (*begin)[1].toString() },
                        quantites_stream{ (*begin)[2].toString() };
                std::get<0>(data_entry) = items_stream.split(";", QString::SkipEmptyParts);
                std::get<1>(data_entry) = prices_stream.split(";", QString::SkipEmptyParts);
                std::get<2>(data_entry) = quantites_stream.split(";", QString::SkipEmptyParts);
                qDebug() << "Entries: " << entries;
               int sum = 0;
                for(int n = 0, size = std::get<0>(data_entry).size(); n < size; ++n)
                {
                    entries[idx][ entries[0].indexOf(std::get<0>(data_entry)[n]) ] = std::get<1>(data_entry)[n];
                    sum += std::get<1>(data_entry)[n].toInt();
                }
                entries[idx][table_header.size()-1] = QString::number(sum);
                total_sales += sum;
            }
            entries.insert(entries.end(), QVector<QString>(table_header.size()));
            entries[entries.size()-1][table_header.size()-1] = QString::number(total_sales);
            sTotalSales << QString::number(total_sales);
            auto qresult = dbController->readFrom("SELECT date FROM dailyEntries WHERE date='%1'", row_id, 1);
            if(!qresult.isEmpty())
                dbController->update("DELETE FROM dailyEntries WHERE date='%1'", row_id, {});
            QList<QStringList> lentries;
            for(const auto& elm : entries)
                lentries << elm.toList();
            staff_entries.insert(staff_entries.end(), new models::TableModel(masterViewController, lentries));
        }
        qDebug() << "StaffsOnDuty: " << staffsOnDuty;
        if(!staffsOnDuty.isEmpty())
            dbController->update("INSERT INTO %1 VALUES(?, ?, ?)", "dailyEntries", { row_id, staffsOnDuty.join(";"), sTotalSales.join(";") });
        return staff_entries;
    }
    models::TableModel *weeklySummary()
    {
        QStringList table_header({"S/N", "Date"});

        const int week_length = 7;

        QVector<QVector<QString>> entries(week_length);
        QList<QStringList> lentries;

        auto qresult{ dbController->readFrom("SELECT first_name, last_name FROM %1 WHERE userType='Staff'", "valid_users", 2) };
        if(!qresult.isEmpty())
        {
            for(const auto& elm : qresult)
                table_header << elm.begin()->toStringList().join(" ");
        }
        table_header << "Total";
        lentries << table_header;

        int eSize = table_header.size();

        for(int i = 0; i < week_length; ++i)
            entries[i].resize(eSize);

        qresult = dbController->readFrom("SELECT date, staffsOnDuty, totalSales FROM %1 ORDER BY ROWID DESC LIMIT 7", "dailyEntries", 3);

        if(qresult.isEmpty())
            return {};

        std::tuple<QStringList, QStringList, QStringList> staff_entries;
        for(auto begin = qresult.crbegin(), end = qresult.crend(); begin != end; ++begin)
        {
            std::get<0>(staff_entries) << (*begin)[0].toString();
            std::get<1>(staff_entries) << (*begin)[1].toString();
            std::get<2>(staff_entries) << (*begin)[2].toString();

        }
        weekDateRange = { ((--qresult.end())->cbegin())->toString(), qresult[0].cbegin()->toString() };
        int finalTotal = 0;
        for(int n = 1; n <= qresult.size(); ++n)
        {
            entries[n-1][0] = QString::number(n);
            entries[n-1][1] = std::get<0>(staff_entries)[n-1];
            QStringList staffsOnDuty{ std::get<1>(staff_entries)[n-1].split(";", QString::SkipEmptyParts) },
                        totalSales{ std::get<2>(staff_entries)[n-1].split(";", QString::SkipEmptyParts) };
            int totalStaffSales = 0;
            for(int i = 0; i < staffsOnDuty.size(); ++i)
            {
                int entryIndex = table_header.indexOf(staffsOnDuty[i]);
                if(entryIndex != -1)
                    entries[n-1][entryIndex] = totalSales[i];
                totalStaffSales += totalSales[i].toInt();
            }
            entries[n-1][table_header.size()-1] = QString::number(totalStaffSales);
            lentries << entries[n-1].toList();
            finalTotal += totalStaffSales;
        }
        entries.insert(entries.end(), QVector<QString>(table_header.size()));
        entries[entries.size()-1][table_header.size()-1] = QString::number(finalTotal);
        lentries << entries[entries.size()-1].toList();
        return new models::TableModel(masterViewController, lentries);
    }
    models::TableModel *annualSummary()
    {
        QStringList table_header({"S/N", "Date"});

        const int year_length = 365 + QDate::isLeapYear(QDate::currentDate().toString("yyyy").toInt());

        QVector<QVector<QString>> entries(year_length);
        QList<QStringList> lentries;

        auto qresult{ dbController->readFrom("SELECT first_name, last_name FROM %1 WHERE userType='Staff'", "valid_users", 2) };
        if(!qresult.isEmpty())
        {
            for(const auto& elm : qresult)
                table_header << elm.begin()->toStringList().join(" ");
        }
        table_header << "Total";
        lentries << table_header;

        int eSize = table_header.size();

        for(int i = 0; i < year_length; ++i)
            entries[i].resize(eSize);

        qresult = dbController->readFrom("SELECT date, staffsOnDuty, totalSales FROM %1 ORDER BY ROWID DESC LIMIT " + QString::number(year_length), "dailyEntries", 3);

        if(qresult.isEmpty())
            return {};

        std::tuple<QStringList, QStringList, QStringList> staff_entries;
        for(auto begin = qresult.crbegin(), end = qresult.crend(); begin != end; ++begin)
        {
            std::get<0>(staff_entries) << (*begin)[0].toString();
            std::get<1>(staff_entries) << (*begin)[1].toString();
            std::get<2>(staff_entries) << (*begin)[2].toString();

        }
        yearDateRange = { ((--qresult.end())->cbegin())->toString(), qresult[0].cbegin()->toString() };
        int finalTotal = 0;
        for(int n = 1; n <= qresult.size(); ++n)
        {
            entries[n-1][0] = QString::number(n);
            entries[n-1][1] = std::get<0>(staff_entries)[n-1];
            QStringList staffsOnDuty{ std::get<1>(staff_entries)[n-1].split(";", QString::SkipEmptyParts) },
                        totalSales{ std::get<2>(staff_entries)[n-1].split(";", QString::SkipEmptyParts) };
            int totalStaffSales = 0;
            for(int i = 0; i < staffsOnDuty.size(); ++i)
            {
                int entryIndex = table_header.indexOf(staffsOnDuty[i]);
                if(entryIndex != -1)
                    entries[n-1][entryIndex] = totalSales[i];
                totalStaffSales += totalSales[i].toInt();
            }
            entries[n-1][table_header.size()-1] = QString::number(totalStaffSales);
            lentries << entries[n-1].toList();
            finalTotal += totalStaffSales;
        }
        entries.insert(entries.end(), QVector<QString>(table_header.size()));
        entries[entries.size()-1][table_header.size()-1] = QString::number(finalTotal);
        lentries << entries[entries.size()-1].toList();
        return new models::TableModel(masterViewController, lentries);
    }
};

MasterViewController::MasterViewController(QObject *parent, IDBController *dbController)
    : QObject(parent)
{
    implementation.reset(new Implementation(this, dbController));
    QObject::connect(this, &MasterViewController::updateMenu, this, &MasterViewController::addToDataBase);
    QObject::connect(this, &MasterViewController::updateStaffMetadata, implementation->toolTipDescriptionController, &ToolTipDescriptionController::rebuildToolTipDescription);
}

MasterViewController::~MasterViewController()
{
}
MenuFormController *MasterViewController::menuFormController()
{
    return implementation->menuFormController;
}
ToolTipDescriptionController *MasterViewController::toolTipDescription()
{
    return implementation->toolTipDescriptionController;
}
QObject *MasterViewController::staffsOnDuty()const
{
    return implementation->staffsOnDutyModel;
}
QList<QObject *> MasterViewController::dailySummary()
{
    return implementation->dailySummary();
}
QList<QObject *> MasterViewController::summaryOn(const QString& date)const
{
    return implementation->summaryOn(date);
}
QObject *MasterViewController::weeklySummary()const
{
    return implementation->weeklySummary();
}
QObject *MasterViewController::annualSummary()const
{
    return implementation->annualSummary();
}
void MasterViewController::addToDataBase()
{
    if(implementation->addToDatabase())
    {
        emit menuAdded({implementation->menuFormController->menuName(),
                          implementation->menuFormController->menuType() == MenuFormController::MenuType::Food ? "Food" : "Drink",
                          implementation->menuFormController->menuPrice()});
    }
}
QObject *MasterViewController::staffs()
{
    return implementation->staffs();
}
QObject *MasterViewController::staffsOnDutyOn(const QString& date)const
{
    return implementation->staffsOnDutyOn(date);
}
QObject *MasterViewController::foods()
{
    return implementation->foods();
}
QObject *MasterViewController::drinks()
{
    return implementation->drinks();
}
void MasterViewController::removeMenu(MenuType menuType, int index)
{
    if(implementation->removeMenu(menuType, index))
        emit menuRemoved({implementation->menuFormController->menuName(),
                          implementation->menuFormController->menuType() == MenuFormController::MenuType::Food ? "Food" : "Drink",
                          ""
                          ""});
}
void MasterViewController::sackStaff(int index, const QString& reason)
{
     if(implementation->sackStaff(index, reason))
         emit updateStaffMetadata();
}
void MasterViewController::updateStaffModel(const QString& staffName)
{
    implementation->updateStaff(staffName);
}
QString MasterViewController::date()const
{
    return "Date: " + QDate::currentDate().toString("ddd MMMM d yyyy");
}
QString MasterViewController::weekDate()const
{
    return "From: " + implementation->weekDateRange.first + "  to: " + implementation->weekDateRange.second;
}
QString MasterViewController::annualDate()const
{
    return {};
}
bool MasterViewController::isStaffActive(int index)const
{
    return implementation->isStaffActive(index);
}

}
}
