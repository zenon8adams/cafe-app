#include "login-form-controller.h"
#include <QDebug>

namespace cafe {
namespace controllers {

class LogInFormController::Implementation {
public:
    Implementation(LogInFormController *_logInFormController)
        :  user(UserType::Unknown), logInFormController(_logInFormController)
    {
    }
    QString userID, password;
    UserType user;
    LogInFormController *logInFormController{ nullptr};
};

LogInFormController::LogInFormController(QObject *parent)
    : QObject(parent)
{
    implementation.reset(new Implementation(this));
}
LogInFormController::~LogInFormController()
{
}

const QString& LogInFormController::userID()const
{
    return implementation->userID;
}

const QString& LogInFormController::password()const
{
    return implementation->password;
}

void LogInFormController::setPassword(const QString& password)
{
    if(implementation->password == password)
        return;
    implementation->password = password;
    emit passwordValueChanged(password);
}

void LogInFormController::setUserID(const QString& userID)
{
    if(implementation->userID == userID)
        return;
    implementation->userID = userID;
    emit userIDValueChanged(userID);
}

void LogInFormController::setUserType(UserType user)
{
    qDebug() <<"User:"<< user;
    if(implementation->user == user)
        return;
    implementation->user = user;
    emit userTypeChanged(user);
}

LogInFormController::UserType LogInFormController::user()const
{
    return implementation->user;
}

}
}
