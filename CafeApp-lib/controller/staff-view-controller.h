#ifndef STAFFVIEWCONTROLLER_H
#define STAFFVIEWCONTROLLER_H

#include <QObject>
#include <QScopedPointer>
#include <controller/idb-controller.h>
#include <model/menu-list-model.h>
#include <model/table-model.h>
#include <cafeapp-lib_global.h>
#include <QQmlListProperty>

namespace cafe {
namespace controllers {

class CAFEAPPLIBSHARED_EXPORT StaffViewController : public QObject
{
    Q_OBJECT

public:

    enum class MenuType {
        Food,
        Drink
    };
    Q_ENUM(MenuType)

    StaffViewController(QObject *parent = nullptr, IDBController *dbController = nullptr);
    ~StaffViewController();
    Q_INVOKABLE QObject *foodModel();
    Q_INVOKABLE QObject *drinkModel();
    Q_INVOKABLE QObject *recieptModel();
    Q_INVOKABLE QString computeTotal();
public slots:
    void setUserID(const QString& id);
    void addToModel(const QStringList&);
    void removeFromModel(const QStringList&);
    void clearData();
    void addPurchaseToDailyEntries();
    Q_INVOKABLE void removeMenu(MenuType menuType, int index);
    Q_INVOKABLE void addToMenu(MenuType menuType, const QString&);
    Q_INVOKABLE void changeMenuPrice(MenuType menuType, const QString &data, int newPrice);
signals:
    void userIDValueChanged(const QString& id);
    void updateDailyEntries();
private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};
}
}
#endif // STAFFVIEWCONTROLLER_H
