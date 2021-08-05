#ifndef LOGININFORMCONTROLLER_H
#define LOGININFORMCONTROLLER_H

#include <QObject>
#include <QScopedPointer>
#include <cafeapp-lib_global.h>

namespace cafe {
namespace controllers {

class CAFEAPPLIBSHARED_EXPORT LogInFormController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString ui_userID READ userID WRITE setUserID NOTIFY userIDValueChanged)
    Q_PROPERTY(QString ui_password READ password WRITE setPassword NOTIFY passwordValueChanged)
   // Q_PROPERTY(UserType ui_userType READ user WRITE setUserType NOTIFY userTypeChanged)
public:

    enum UserType {
        Admin,
        Staff,
        Unknown
    };

    Q_ENUM(UserType)

    explicit LogInFormController(QObject *parent = nullptr);
    ~LogInFormController();
    const QString& userID()const;
    const QString& password()const;
    UserType user()const;

signals:
    void userIDValueChanged(const QString&);
    void passwordValueChanged(const QString&);
    void userTypeChanged(UserType);
    void readyToValidate();
public slots:
    void setUserID(const QString&);
    void setPassword(const QString&);
    void setUserType(UserType user);
private:
    class Implementation;
    QScopedPointer<Implementation> implementation;

};

}
}
#endif // LOGININFORMCONTROLLER_H
