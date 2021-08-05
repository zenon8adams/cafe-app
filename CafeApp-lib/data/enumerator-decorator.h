#ifndef ENUMERATORDECORATOR_H
#define ENUMERATORDECORATOR_H

#include <map>
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

class CAFEAPPLIBSHARED_EXPORT EnumeratorDecorator : public DataDecorator
{
    Q_OBJECT
    Q_PROPERTY(int ui_value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QString ui_valueDescription READ valueDescription NOTIFY valueChanged)

public:
    EnumeratorDecorator(Entity* parentEntity = nullptr, const QString& key = "SomeItemKey",
                        const QString& label = "", int value = {}, const std::map<int, QString>& descriptionMapper = {});
    ~EnumeratorDecorator();

    EnumeratorDecorator& setValue(int);
    int value()const;
    QString valueDescription()const;

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
#endif // ENUMERATORDECORATOR_H
