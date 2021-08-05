#ifndef SIGNUPFORMCONTROLLER_H
#define SIGNUPFORMCONTROLLER_H

#include <QObject>
#include <QScopedPointer>
#include <cafeapp-lib_global.h>

namespace cafe {
namespace controllers {

class CAFEAPPLIBSHARED_EXPORT SignUpFormController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ui_firstName READ firstName WRITE setFirstName NOTIFY firstNameValueChanged)
    Q_PROPERTY(QString ui_lastName READ lastName WRITE setLastName NOTIFY lastNameValueChanged)
    Q_PROPERTY(QString ui_userID READ userID WRITE setUserID NOTIFY userIDValueChanged)
    Q_PROPERTY(QString ui_password READ password WRITE setPassword NOTIFY passwordValueChanged)
    Q_PROPERTY(QString adminPassword READ adminPassword WRITE setAdminPassword NOTIFY adminPasswordValueChanged)
public:
    explicit SignUpFormController(QObject *parent = nullptr);
    ~SignUpFormController();
    const QString& firstName()const;
    const QString& lastName()const;
    const QString& userID()const;
    const QString& password()const;
    const QString& adminPassword()const;
    bool emptyFields()const;
signals:
    void userIDValueChanged(const QString&);
    void passwordValueChanged(const QString&);
    void firstNameValueChanged(const QString&);
    void lastNameValueChanged(const QString&);
    void adminPasswordValueChanged(const QString&);
    void readyToValidate();
public slots:
    void setFirstName(const QString&);
    void setLastName(const QString&);
    void setUserID(const QString&);
    void setPassword(const QString&);
    void setAdminPassword(const QString&);
private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};

}
}
#endif // SIGNUPFORMCONTROLLER_H
