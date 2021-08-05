#ifndef DATETIMEDECORATOR_H
#define DATETIMEDECORATOR_H


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


class CAFEAPPLIBSHARED_EXPORT DateTimeDecorator : public DataDecorator
{
    Q_OBJECT
    Q_PROPERTY(QDateTime ui_value READ value WRITE setValue NOTIFY valueChanged)

    Q_PROPERTY(QString ui_iso8601String READ toiso8601String NOTIFY valueChanged)
    Q_PROPERTY(QString ui_prettyDateString READ toprettyDateString NOTIFY valueChanged)
    Q_PROPERTY(QString ui_prettyTimeString READ toprettyTimeString NOTIFY valueChanged)
    Q_PROPERTY(QString ui_prettyString READ toprettyString NOTIFY valueChanged)

public:
    DateTimeDecorator(Entity* parentEntity = nullptr, const QString& key = "SomeKeyItem", const QString& label = "", const QDateTime& value = {});
    ~DateTimeDecorator()override;

    DateTimeDecorator& setValue(const QDateTime& value);
    const QDateTime& value()const;
    QString toiso8601String()const;
    QString toprettyDateString()const;
    QString toprettyTimeString()const;
    QString toprettyString()const;

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
#endif // DATETIMEDECORATOR_H
