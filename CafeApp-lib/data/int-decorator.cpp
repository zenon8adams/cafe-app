#include "int-decorator.h"

namespace cafe {
namespace data {

class IntDecorator::Implementation
{
public:

    Implementation(IntDecorator* _IntDecorator, int _value)
                  : IntDecorator(_IntDecorator), value(_value)
    {
    }

    IntDecorator *IntDecorator{ nullptr };
    int value;
};

IntDecorator::IntDecorator(Entity* parentEntity, const QString& key, const QString& label, int value)
                                : DataDecorator(parentEntity, key, label)
{
    implementation.reset( new Implementation(this, value));
}

IntDecorator::~IntDecorator()
{
}

IntDecorator& IntDecorator::setValue(int value)
{
    if(implementation->value != value){
        implementation->value = value;
        emit valueChanged();
    }
    return *this;
}
int IntDecorator::value()const
{
    return implementation->value;
}

QJsonValue IntDecorator::jsonValue()const
{
    return QJsonValue::fromVariant(QVariant(value()));
}

void IntDecorator::update(const QJsonObject& _jsonObject)
{
    if(_jsonObject.contains(key()))
        setValue(_jsonObject.value(key()).toInt());
    else
        setValue({});
}

}
}
