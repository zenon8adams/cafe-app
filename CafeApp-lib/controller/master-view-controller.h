#ifndef MASTERVIEWCONTROLLER_H
#define MASTERVIEWCONTROLLER_H

#include <QObject>
#include <QScopedPointer>
#include <controller/idb-controller.h>
#include <controller/menu-form-controller.h>
#include <model/table-model.h>
#include <cafeapp-lib_global.h>
#include <controller/tooltip-description-controller.h>

namespace cafe {
namespace controllers {

class CAFEAPPLIBSHARED_EXPORT MasterViewController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(MenuFormController* ui_menuFormController READ menuFormController)
    Q_PROPERTY(ToolTipDescriptionController* ui_toolTipDescription READ toolTipDescription CONSTANT)
    Q_PROPERTY(QString ui_date READ date CONSTANT)
    Q_PROPERTY(QString ui_weekDate READ weekDate CONSTANT)
    Q_PROPERTY(QString ui_annualDate READ annualDate CONSTANT)

public:

    enum class MenuType {
        Food,
        Drink
    };
    Q_ENUM(MenuType)

    MasterViewController(QObject *parent = nullptr, IDBController *dbController = nullptr);
    ~MasterViewController();

    QString date()const;
    QString weekDate()const;
    QString annualDate()const;

    Q_INVOKABLE QList<QObject *> dailySummary();
    Q_INVOKABLE QList<QObject *> summaryOn(const QString& date) const;
    Q_INVOKABLE QObject *weeklySummary()const;
    Q_INVOKABLE QObject *annualSummary()const;
    MenuFormController *menuFormController();
    ToolTipDescriptionController *toolTipDescription();
    Q_INVOKABLE QObject *staffsOnDuty()const;
    Q_INVOKABLE QObject *staffs();
    Q_INVOKABLE QObject *staffsOnDutyOn(const QString& date)const;
    Q_INVOKABLE QObject *foods();
    Q_INVOKABLE QObject *drinks();
    Q_INVOKABLE void removeMenu(MenuType menuType, int index);
    Q_INVOKABLE bool isStaffActive(int index)const;
    //QString& staffOfTheWeek()const;
public slots:
    void addToDataBase();
    void updateStaffModel(const QString&);
    void sackStaff(int index, const QString& reason = "");
signals:
    void menuRemoved(const QStringList&);
    void menuAdded(const QStringList&);
    void updateMenu();
    void updateStaffMetadata();
private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};

}
}

#endif // MASTERVIEWCONTROLLER_H
