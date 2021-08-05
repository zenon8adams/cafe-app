#include "enumerator-decorator.h"

namespace cafe {
namespace data {

class EnumeratorDecorator::Implementation
{
public:

    Implementation(EnumeratorDecorator* _enumeratorDecorator, int _value, const std::map<int, QString>& _descriptionMapper)
                  : enumeratorDecorator(_enumeratorDecorator), value(_value), descriptionMapper(_descriptionMapper)
    {
    }
    int value{};
    EnumeratorDecorator *enumeratorDecorator{ nullptr };
    std::map<int, QString> descriptionMapper;
};

EnumeratorDecorator::EnumeratorDecorator(Entity* parentEntity, const QString& key,
                                         const QString& label, int value, const std::map<int, QString>& descriptionMapper)
                                        : DataDecorator(parentEntity, key, label)
{
    implementation.reset(new Implementation(this, value, descriptionMapper));
}

EnumeratorDecorator::~EnumeratorDecorator()
{
}

EnumeratorDecorator& EnumeratorDecorator::setValue(int value)
{
    if(implementation->value != value)
        implementation->value = value;

    emit valueChanged();
    return *this;
}
int EnumeratorDecorator::value()const
{
    return  implementation->value;
}
QString EnumeratorDecorator::valueDescription()const
{
    if(implementation->descriptionMapper.find(value()) != implementation->descriptionMapper.cend())
        return implementation->descriptionMapper[value()];
    return {};
}

QJsonValue EnumeratorDecorator::jsonValue()const
{
    return QJsonValue::fromVariant(QVariant(value()));
}

void EnumeratorDecorator::update(const QJsonObject& _jsonObject)
{
    if(_jsonObject.contains(key()))
        setValue(_jsonObject.value(key()).toInt());
    else
        setValue({});
}

}
}
