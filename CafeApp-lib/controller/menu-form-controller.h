#ifndef MENUFORMCONTROLLER_H
#define MENUFORMCONTROLLER_H

#include <QObject>
#include <QScopedPointer>
#include <cafeapp-lib_global.h>

namespace cafe {
namespace controllers {

class CAFEAPPLIBSHARED_EXPORT MenuFormController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(MenuType ui_menuType READ menuType WRITE setMenuType NOTIFY menuTypeChanged)
    Q_PROPERTY(QString ui_menuName READ menuName WRITE setMenuName NOTIFY menuNameChanged)
    Q_PROPERTY(QString ui_menuPrice READ menuPrice WRITE setMenuPrice NOTIFY menuPriceChanged)


public:
    enum MenuType {
        Food,
        Drink,
        Unknown
    };

    Q_ENUM(MenuType)

    explicit MenuFormController(QObject *parent = nullptr);
    ~MenuFormController();
    MenuType menuType()const;
    const QString& menuName()const;
    const QString& menuPrice()const;

signals:
    void menuTypeChanged(MenuType);
    void menuPriceChanged(const QString&);
    void menuNameChanged(const QString&);
public slots:
    void setMenuType(MenuType);
    void setMenuName(const QString&);
    void setMenuPrice(const QString&);
private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};

}
}
#endif // MENUFORMCONTROLLER_H
