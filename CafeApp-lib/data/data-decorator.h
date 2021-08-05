#ifndef DATADECORATOR_H
#define DATADECORATOR_H

#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QScopedPointer>
#include <QDateTime>

#include <cafeapp-lib_global.h>

namespace cafe {
namespace data {

class Entity;

class CAFEAPPLIBSHARED_EXPORT DataDecorator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ui_label READ label CONSTANT)

public:
    DataDecorator(Entity *parent = nullptr, const QString& key = "SomeKeyItem", const QString& label = "");

    virtual ~DataDecorator();

    const QString& key()const;
    const QString& label()const;
    Entity* parentEntity();

    virtual QJsonValue jsonValue()const = 0;
    virtual void update(const QJsonObject&) = 0;

private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};

}
}

#endif // DATADECORATOR_H
