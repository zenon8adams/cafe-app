#ifndef MASTERCONTROLLER_H
#define MASTERCONTROLLER_H

#include <QObject>
#include <QScopedPointer>
#include <cafeapp-lib_global.h>

#include <controller/login-form-controller.h>
#include <controller/signup-form-controller.h>
#include <controller/master-view-controller.h>
#include <controller/staff-view-controller.h>
#include <controller/idb-controller.h>
#include <framework/error-handler.h>
#include <utility/rsa-encryption.h>

namespace cafe {
namespace controllers {

class CAFEAPPLIBSHARED_EXPORT MasterController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(LogInFormController* ui_logInFormController READ logInFormController CONSTANT)
    Q_PROPERTY(LogInFormController* ui_adminRegController READ adminRegistrationController CONSTANT)
    Q_PROPERTY(SignUpFormController* ui_signUpFormController READ signUpFormController CONSTANT)
    Q_PROPERTY(MasterViewController* ui_masterViewController READ createMasterView CONSTANT)
    Q_PROPERTY(StaffViewController* ui_staffViewController READ createStaffView CONSTANT)
    Q_PROPERTY(bool ui_validLoginDetails READ validLoginDetails CONSTANT)
    Q_PROPERTY(bool ui_validSignupDetails READ validSignupDetails CONSTANT)
public:
    explicit MasterController(QObject *parent = nullptr);
    ~MasterController();
    LogInFormController *logInFormController();
    LogInFormController *adminRegistrationController();
    SignUpFormController *signUpFormController();
    utility::RSAEncryption *createAppEncryption();
    Q_INVOKABLE QObject *errorHandler();
    Q_INVOKABLE bool registerAdmin();
    Q_INVOKABLE bool validLicense(const QString &license);
    Q_INVOKABLE bool firstTimeAppLaunch()const;
    Q_INVOKABLE bool initAdmin();
    bool validLoginDetails()const;
    bool validSignupDetails()const;
    MasterViewController *createMasterView();
    StaffViewController *createStaffView();

signals:
    void staffAdded(const QString&)const;
    void staffReactivated()const;
    void updateLogs()const;
public slots:
private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};

}
}

#endif
