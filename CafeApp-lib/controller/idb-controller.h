#ifndef IDBCONTROLLER_H
#define IDBCONTROLLER_H

#include <QObject>
#include <QVariantList>
#include <cafeapp-lib_global.h>

class CAFEAPPLIBSHARED_EXPORT IDBController : public QObject
{
    Q_OBJECT
public:
    explicit IDBController(QObject *parent = nullptr)
        : QObject(parent)
    {
    }
    virtual ~IDBController()
    {
    }
    virtual bool createTable(const QString& cmd, const QString& tableName) = 0;
    virtual QList<QVariantList> readFrom(const QString& cmd, const QString& otherArgs, int numArgs) = 0;
    virtual bool update(const QString& cmd, const QString& tableName, const QVariantList& value) = 0;
    virtual bool deleteFrom(const QString& cmd, const QString& args) = 0;
signals:

public slots:
};

#endif // IDBCONTROLLER_H
