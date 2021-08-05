#ifndef INTDECORATOR_H
#define INTDECORATOR_H

#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QScopedPointer>
#include <QString>
#include <QVariant>

#include <cafeapp-lib_global.h>
#include <data/data-decorator.h>

namespace cafe {
namespace data {


class CAFEAPPLIBSHARED_EXPORT IntDecorator : public DataDecorator
{
    Q_OBJECT
    Q_PROPERTY(int ui_value READ value WRITE setValue NOTIFY valueChanged)

public:
    IntDecorator(Entity* parentEntity = nullptr, const QString& key = "SomeKeyItem", const QString& label = "", int value = {});
    ~IntDecorator();

    IntDecorator& setValue(int value);
    int value()const;

    virtual QJsonValue jsonValue()const override;
    virtual void update(const QJsonObject&) override;
signals:

    void valueChanged();
private:

    class Implementation;
    QScopedPointer<Implementation> implementation;
};

}
}

#endif // INTDECORATOR_H
