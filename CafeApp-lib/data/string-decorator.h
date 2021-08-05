#ifndef STRINGDECORATOR_H
#define STRINGDECORATOR_H

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

class CAFEAPPLIBSHARED_EXPORT StringDecorator : public DataDecorator
{
    Q_OBJECT
    Q_PROPERTY(QString ui_value READ value WRITE setValue NOTIFY valueChanged)

public:
    StringDecorator(Entity* parentEntity = nullptr, const QString& key = "SomeItemKey", const QString& label = "", const QString& value = "");
    ~StringDecorator();

    StringDecorator& setValue(const QString& value);
    const QString& value()const;

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
#endif // STRINGDECORATOR_H
