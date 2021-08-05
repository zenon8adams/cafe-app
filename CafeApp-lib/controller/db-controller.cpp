#include "db-controller.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <qstandardpaths.h>
#include <qsettings.h>
#include <qrandom.h>
#include <quuid.h>
#include <qdir.h>

class DBController::Implementation
{
public:
    Implementation(DBController *_dbController)
        : dbController(_dbController)
    {
        init();
        Q_UNUSED(dbController)
    }
    bool init()
    {
        constexpr const char *appname = "CafeApp";
        QSettings dbs = QSettings( QString( "HKEY_CURRENT_USER\\Software\\") + appname, QSettings::NativeFormat);
        QString db_nm = dbs.value( "Cryptography/DatabaseName").toString(),
                db_id = dbs.value( "Cryptography/DatabaseID").toString(),
                dbname = db_nm,
                dbpass = db_id,
                username = qgetenv( "USER"), db_path;

        username = username.isEmpty() ? qgetenv( "USERNAME") : username;
        username = username.isEmpty() ? "user" : username;
        db_path.append( "C:/Users/");
        db_path.append( username + "/AppData/Local/");
        db_path.append( appname);

        qDebug() << "db_nm: " << db_nm
                 << "db_id: " << db_id;

        if( db_nm.isEmpty() || db_id.isEmpty())
        {
            QString option = "KLMcdXklABNmaSTIjEGHqrPQUIJnopefghWbRFCDYstuvwxyzOZV";
            auto chosen = option[ QRandomGenerator::system()->generate() % quint32( option.size()) ];
            dbname = QString( QSysInfo::machineUniqueId()).remove('-').prepend( chosen);
            dbpass = QUuid::createUuid().toString();

            qDebug() << "dbname: " << dbname
                     << "dbpass: " << dbpass;

            dbs.setValue( "Cryptography/DatabaseName", dbname);
            dbs.setValue( "cryptography/DatabaseID", dbpass);

        }else if( db_nm.mid( 1) != QString( QSysInfo::machineUniqueId()).remove('-'))
            return false;

        QDir().mkpath( db_path);

        qDebug() << "db: " << db_path + "/" + dbname + ".db";

        database = QSqlDatabase::addDatabase( "QSQLITE", appname);
        database.setDatabaseName( db_path + "/" + dbname + ".db");
        database.setUserName( dbname);
        return database.open(database.userName(), dbpass);
    }
    bool create(const QString& cmd, const QString& tableName)
    {
        QSqlQuery query(cmd.arg(tableName), database);
        return query.exec();
    }
    QList<QVariantList> read(const QString& cmd, const QString& otherArgs, int numArgs)
    {
        qDebug() << "Read Command:" << cmd.arg(otherArgs);

        QSqlQuery query(cmd.arg(otherArgs), database);
        if(query.exec())
        {
            QList<QVariantList> cols;
            while(query.next())
            {
                QList<QVariant> row;
                for(int n = 0; n < numArgs; ++n)
                    row << query.value(n);
                cols << row;
            }
            return cols;
        }
        return {};
    }
    bool update(const QString& cmd, const QString& tableName, const QVariantList& value)
    {
        qDebug() << "Command given:" << cmd.arg(tableName);

        QSqlQuery query(cmd.arg(tableName), database);
        int count = 0;
        for(const auto& elm : value)
            query.bindValue(count++, elm);
        bool exec = query.exec();
        qDebug() << "Exec:" << exec;
        qDebug() << "Last Error:" << query.lastError().text();
        qDebug() << "Count:" << count;
        qDebug() << "Input:" << value;
        return exec;
    }
    bool deleteEntry(const QString& cmd, const QString& args)
    {
        QSqlQuery query(cmd.arg(args), database);
        return query.exec();
    }

private:

    QSqlDatabase database;
    DBController *dbController { nullptr };

};

DBController::DBController(QObject *parent)
    : IDBController (parent)
{
    implementation.reset(new Implementation(this));
}
DBController::~DBController()
{
}

bool DBController::createTable(const QString& cmd, const QString& tableName)
{
    return implementation->create(cmd, tableName);
}
QList<QList<QVariant>> DBController::readFrom(const QString& cmd, const QString& otherArgs, int numArgs)
{
    return implementation->read(cmd, otherArgs, numArgs);
}
bool DBController::update(const QString& cmd, const QString& tableName, const QVariantList& value)
{
    return implementation->update(cmd, tableName, value);
}
bool DBController::deleteFrom(const QString& cmd, const QString &args)
{
    return implementation->deleteEntry(cmd, args);
}
