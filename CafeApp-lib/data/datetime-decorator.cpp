#include "datetime-decorator.h"

namespace cafe {
namespace data {

class DateTimeDecorator::Implementation
{
public:

    Implementation(DateTimeDecorator* _DateTimeDecorator, const QDateTime& _value)
                  : DateTimeDecorator(_DateTimeDecorator), value(_value)
    {
    }

    DateTimeDecorator *DateTimeDecorator{ nullptr };
    QDateTime value;
};

DateTimeDecorator::DateTimeDecorator(Entity* parentEntity, const QString& key, const QString& label, const QDateTime& value)
                                : DataDecorator(parentEntity, key, label)
{
    implementation.reset( new Implementation(this, value));
}

DateTimeDecorator::~DateTimeDecorator()
{
}

DateTimeDecorator& DateTimeDecorator::setValue(const QDateTime& value)
{
    if(implementation->value != value)
        implementation->value = value;

    emit valueChanged();
    return *this;
}
const QDateTime& DateTimeDecorator::value()const
{
    return implementation->value;
}
QString DateTimeDecorator::toiso8601String()const
{
    if(implementation->value.isNull())
        return "";
    return implementation->value.toString(Qt::ISODate);
}
QString DateTimeDecorator::toprettyDateString()const
{
    if(implementation->value.isNull())
        return "Not set";
    return implementation->value.toString("ddd d MMM  yyyy @ HH:mm:ss");
}
QString DateTimeDecorator::toprettyTimeString()const
{
    if(implementation->value.isNull())
        return "Not set";
    return implementation->value.toString("hh:mm ap");
}
QString DateTimeDecorator::toprettyString()const
{
    if(implementation->value.isNull())
        return "Not set";
    return implementation->value.toString("d MMM yyyy");
}

QJsonValue DateTimeDecorator::jsonValue()const
{
    return QJsonValue::fromVariant(QVariant(value()));
}
void DateTimeDecorator::update(const QJsonObject& _jsonObject)
{
    if(_jsonObject.contains(key()))
        setValue(QDateTime::fromString(_jsonObject.value(key()).toString()));
    else
        setValue(QDateTime());
}

}
}
