#include "signup-form-controller.h"
#include <QDebug>

namespace cafe {
namespace controllers {

class SignUpFormController::Implementation {
public:
    Implementation(SignUpFormController *_SignUpFormController)
        : SignUpFormController(_SignUpFormController)
    {
    }
    QString firstName, lastName, userID, password, adminPassword;
    SignUpFormController *SignUpFormController;
    bool emptyFields()
    {
        return and_all(firstName, lastName, userID, password, adminPassword);
    }
    template<typename Tp_>
    bool emptyField(const Tp_& field)const
    {
        return field.isEmpty();
    }
    bool and_all()const
    {
        return true;
    }
    template<typename First, typename... Others>
    bool and_all(const First& first, const Others&... others)const
    {
        return emptyField(first) && and_all(others...);
    }

};

SignUpFormController::SignUpFormController(QObject *parent)
    : QObject(parent)
{
    implementation.reset(new Implementation(this));
}
SignUpFormController::~SignUpFormController()
{
}

const QString& SignUpFormController::userID()const
{
    return implementation->userID;
}

const QString& SignUpFormController::password()const
{
    return implementation->password;
}

const QString& SignUpFormController::adminPassword()const
{
    return implementation->adminPassword;
}
bool SignUpFormController::emptyFields()const{
    return implementation->emptyFields();
}
const QString& SignUpFormController::firstName()const
{
    return implementation->firstName;
}

const QString& SignUpFormController::lastName()const
{
    return implementation->lastName;
}


void SignUpFormController::setFirstName(const QString& firstName)
{
    if(implementation->firstName == firstName)
        return;
    implementation->firstName = firstName;
    emit firstNameValueChanged(firstName);
}


void SignUpFormController::setLastName(const QString& lastName)
{
    if(implementation->lastName == lastName)
        return;
    implementation->lastName = lastName;
    emit lastNameValueChanged(lastName);
}

void SignUpFormController::setPassword(const QString& password)
{
    if(implementation->password == password)
        return;
    implementation->password = password;
    emit passwordValueChanged(password);
}

void SignUpFormController::setAdminPassword(const QString& password)
{
    if(implementation->adminPassword == password)
        return;
    implementation->adminPassword = password;
    emit adminPasswordValueChanged(password);
}

void SignUpFormController::setUserID(const QString& userID)
{
    if(implementation->userID == userID)
        return;
    implementation->userID = userID;
    emit userIDValueChanged(userID);
}

}
}
