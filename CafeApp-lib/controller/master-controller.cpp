#include "master-controller.h"
#include <controller/db-controller.h>
#include <QDate>
#include <QDebug>


namespace cafe {
namespace controllers {

class MasterController::Implementation {

public:
  Implementation(MasterController *_masterController)
    : masterController(_masterController)
  {
      dbController                = new DBController(masterController);
      rsaEncryption               = new utility::RSAEncryption({29, 47}, masterController);
      errorHandler                = new framework::ErrorHandler(masterController);
      logInFormController         = new LogInFormController(masterController);
      adminRegistrationController = new LogInFormController(masterController);
      signUpFormController        = new SignUpFormController(masterController);
      masterViewController        = new MasterViewController(masterController, dbController);
      staffViewController         = new StaffViewController(masterController, dbController);

      QObject::connect(logInFormController, &LogInFormController::readyToValidate, masterController, &MasterController::validLoginDetails);
      QObject::connect(signUpFormController, &SignUpFormController::readyToValidate, masterController, &MasterController::validSignupDetails);
  }
  MasterController        *masterController{ nullptr };
  LogInFormController     *logInFormController{ nullptr },
                          *adminRegistrationController{ nullptr };
  SignUpFormController    *signUpFormController{ nullptr };
  DBController            *dbController{ nullptr };
  MasterViewController    *masterViewController{ nullptr };
  StaffViewController     *staffViewController{ nullptr };
  framework::ErrorHandler *errorHandler{ nullptr };
  utility::RSAEncryption  *rsaEncryption{ nullptr };
  QString                 adminPassword, pub_key;
  void initialize()
  {
      dbController->createTable("CREATE TABLE IF NOT EXISTS %1(id TEXT UNIQUE NOT NULL, userType TEXT, first_name TEXT, last_name TEXT, passkey TEXT, state TEXT, joinedOn TEXT, leftOn TEXT, ReasonForLeaving TEXT)", "valid_users");
      dbController->createTable("CREATE TABLE IF NOT EXISTS %1(id INTEGER PRIMARY KEY, items text UNIQUE, itemType TEXT, price INTEGER, available TEXT, addedOn TEXT, removedOn TEXT)", "valid_menu");
      dbController->createTable("CREATE TABLE IF NOT EXISTS %1(date TEXT, staffsOnDuty TEXT, totalSales TEXT)", "dailyEntries");

  }
  void initAdmin(const QString& id, const QString& password)
  {
      initialize();

      auto qresult{ dbController->readFrom("SELECT passkey FROM valid_users WHERE userType='%1'", "Admin", 1) };
      qDebug() <<"Returned Statment - "<< qresult;
      if(!qresult.isEmpty())
      {
          QString tmp{ qresult[0][0].toString() };
          pub_key = rsaEncryption->extractKey(tmp);
          rsaEncryption->setPublicKey(pub_key);
          adminPassword = rsaEncryption->decrypt(qresult[0][0].toString());
          return;
      }
      if(id.isEmpty() && password.isEmpty())
          return;
      dbController->update("insert into %1 values(?, ?, ?, ?, ?, ?, ?, ?, ?)", "valid_users", {id, "Admin", "", "", rsaEncryption->encrypt(password, 64, true), "active", QDate::currentDate().toString("dd-MM-yyyy"), "", ""});
      adminPassword = password;

  }
  bool createAdmin()
  {
      initAdmin(adminRegistrationController->userID(), adminRegistrationController->password());
      return true;
  }
  bool initAdmin()
  {
      initAdmin("", "");
      return true;
  }

};

MasterController::MasterController(QObject *parent) : QObject(parent)
{
    implementation.reset(new Implementation(this));
    QObject::connect(implementation->masterViewController, &MasterViewController::menuAdded, implementation->staffViewController, &StaffViewController::addToModel);
    QObject::connect(implementation->masterViewController, &MasterViewController::menuRemoved, implementation->staffViewController, &StaffViewController::removeFromModel);
    QObject::connect(this, &MasterController::staffAdded, implementation->masterViewController, &MasterViewController::updateStaffModel);
    QObject::connect(this, &MasterController::updateLogs, implementation->staffViewController, &StaffViewController::clearData);
    QObject::connect(this, &MasterController::updateLogs, implementation->masterViewController, &MasterViewController::updateStaffMetadata);
    QObject::connect(this, &MasterController::staffReactivated, implementation->masterViewController, &MasterViewController::updateStaffMetadata);
}

MasterController::~MasterController()
{
}

MasterViewController *MasterController::createMasterView()
{
    return implementation->masterViewController;
}

StaffViewController *MasterController::createStaffView()
{
    return implementation->staffViewController;
}

LogInFormController *MasterController::logInFormController()
{
    return implementation->logInFormController;
}
LogInFormController *MasterController::adminRegistrationController()
{
    return implementation->adminRegistrationController;
}

SignUpFormController *MasterController::signUpFormController()
{
    return implementation->signUpFormController;
}

utility::RSAEncryption *MasterController::createAppEncryption()
{
    return implementation->rsaEncryption;
}

QObject *MasterController::errorHandler()
{
    return implementation->errorHandler;
}
bool MasterController::registerAdmin()
{
    return implementation->createAdmin();
}
bool MasterController::validLicense(const QString& license)
{
    QScopedPointer<utility::RSAEncryption> license_key{ new utility::RSAEncryption({59, 47}, this) };
    auto code = license_key->decrypt(license);
    auto enc_time = code.split(';', QString::SkipEmptyParts);
    if( enc_time.size() != 2)
        return false;
    QDateTime gen_date = QDateTime::fromString(enc_time[0], "dd-MM-yyyy:HH:mm:ss"),
              given_date = QDateTime::fromString(enc_time[1], "dd-MM-yyyy:HH:mm:ss");
    return gen_date < given_date;
}
bool MasterController::firstTimeAppLaunch() const
{
    return implementation->dbController->readFrom("SELECT id FROM %1", "valid_users", 1).isEmpty();
}
bool MasterController::initAdmin()
{
    return implementation->initAdmin();
}
bool MasterController::validLoginDetails()const
{
    QString userType = implementation->logInFormController->user() == implementation->logInFormController->UserType::Admin ?
                        "Admin" : implementation->logInFormController->user() == implementation->logInFormController->UserType::Staff ?
                        "Staff" : "Unknown";
    auto result { implementation->dbController->readFrom("SELECT passkey, userType FROM valid_users WHERE id='%1' AND state='active'", implementation->logInFormController->userID(), 2) };

    if(result.isEmpty())
    {
        implementation->errorHandler->setErrorMsg("Invalid username or password");
        return false;
    }

    if(userType != result[0][1].toString())
    {
        implementation->errorHandler->setErrorMsg("Invalid username or password");
        return false;
    }
    qDebug() << "decrypt result[0][0].toString(): " << implementation->rsaEncryption->decrypt(result[0][0].toString()) << ", password: " << implementation->logInFormController->password();
    if(implementation->rsaEncryption->decrypt(result[0][0].toString()) == implementation->logInFormController->password())
    {
        if(userType == "Staff")
            implementation->staffViewController->setUserID(implementation->logInFormController->userID());
        emit updateLogs();
        return true;
    }else
        implementation->errorHandler->setErrorMsg("Invalid username or password");
    return false;
}

bool MasterController::validSignupDetails()const
{

    if(implementation->signUpFormController->emptyFields())
    {
        implementation->errorHandler->setErrorMsg("Fields cannot be empty!");
        return false;
    }
    if(implementation->adminPassword != implementation->signUpFormController->adminPassword())
    {
        qDebug() << "adminPassword: " << implementation->adminPassword << ", input password: " << implementation->signUpFormController->adminPassword();
        implementation->errorHandler->setErrorMsg("Admin can't vouch for you!");
        return false;
    }
    if(implementation->signUpFormController->userID().isEmpty())
    {
        implementation->errorHandler->setErrorMsg("User ID field cannnot be empty!");
        return false;
    }
    if(implementation->signUpFormController->password().isEmpty())
    {
        implementation->errorHandler->setErrorMsg("Requires password field cannnot be empty!");
        return false;
    }
    auto result { implementation->dbController->readFrom("SELECT state FROM valid_users WHERE id='%1'", implementation->signUpFormController->userID(), 1) };
    if(result.isEmpty())
    {

        implementation->dbController->update("INSERT INTO %1 values(?, ?, ?, ?, ?, ?, ?, ?, ?)", "valid_users", {
                                                                                                        implementation->signUpFormController->userID(),
                                                                                                        "Staff",
                                                                                                        implementation->signUpFormController->firstName(),
                                                                                                        implementation->signUpFormController->lastName(),
                                                                                                        implementation->rsaEncryption->encrypt(implementation->signUpFormController->password(), 64),
                                                                                                        "active",
                                                                                                        QDate::currentDate().toString("dd-MM-yyyy"),
                                                                                                        "",
                                                                                                        ""});
        implementation->dbController->createTable("CREATE TABLE IF NOT EXISTS %1_SalesRecord(date_time TEXT UNIQUE, item_names TEXT, prices TEXT, quantities TEXT)", implementation->signUpFormController->userID());
        implementation->staffViewController->setUserID(implementation->signUpFormController->userID());
        emit staffAdded(implementation->signUpFormController->firstName() + "\n" + implementation->signUpFormController->lastName());
        emit updateLogs();
        return true;
    }else {
        if(result[0][0].toString() == "sacked"){
            emit staffReactivated();
            return implementation->dbController->update("UPDATE valid_users SET state='active', leftOn='', passkey='" + implementation->rsaEncryption->encrypt(implementation->signUpFormController->password()) + "' WHERE id='%1'", implementation->signUpFormController->userID(), {});
        }else {
            implementation->errorHandler->setErrorMsg("User exists already!");
            return false;
        }
    }
}

}
}
