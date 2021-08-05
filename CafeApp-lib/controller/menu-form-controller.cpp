#include "menu-form-controller.h"

namespace cafe {
namespace controllers {

class MenuFormController::Implementation
{
public:
    Implementation(MenuFormController *_menuFormController)
        : menuFormController(_menuFormController)
    {
        Q_UNUSED(menuFormController)
    }
    MenuFormController *menuFormController{ nullptr };
    MenuType menuType;
    QString menuName, menuPrice;
};

MenuFormController::MenuFormController(QObject *parent) : QObject(parent)
{
    implementation.reset(new Implementation(this));
}
MenuFormController::~MenuFormController()
{
}

MenuFormController::MenuType MenuFormController::menuType()const
{
    return implementation->menuType;
}

const QString& MenuFormController::menuName()const
{
    return implementation->menuName;
}

const QString& MenuFormController::menuPrice()const
{
    return implementation->menuPrice;
}

void MenuFormController::setMenuType(MenuType menuType)
{
    if(implementation->menuType == menuType)
        return;
    implementation->menuType = menuType;
    emit menuTypeChanged(menuType);
}


void MenuFormController::setMenuName(const QString& menuName)
{
    if(implementation->menuName == menuName)
        return;
    implementation->menuName = menuName;
    emit menuNameChanged(menuName);
}

void MenuFormController::setMenuPrice(const QString& menuPrice)
{
    if(implementation->menuPrice == menuPrice)
        return;
    implementation->menuPrice = menuPrice;
    emit menuPriceChanged(menuPrice);
}

}
}
