#ifndef DBCONTROLLER_H
#define DBCONTROLLER_H

#include <QObject>
#include <controller/idb-controller.h>
#include <QScopedPointer>
#include <QVariantList>
#include <cafeapp-lib_global.h>

class CAFEAPPLIBSHARED_EXPORT DBController : public IDBController
{
    Q_OBJECT
public:
    DBController(QObject *parent = nullptr);
    ~DBController() override;
    bool createTable(const QString& cmd, const QString& tableName) override;
    QList<QVariantList> readFrom(const QString& cmd, const QString& otherArgs, int numArgs) override;
    bool update(const QString& cmd, const QString& tableName, const QVariantList& value) override;
    bool deleteFrom(const QString& cmd, const QString& pred)override;
private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};

#endif // DBCONTROLLER_H
