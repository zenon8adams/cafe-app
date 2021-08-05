#ifndef ENTITY_H
#define ENTITY_H

#include <map>
#include <QObject>
#include <QScopedPointer>
#include <QJsonArray>

#include <cafeapp-lib_global.h>
#include <data/data-decorator.h>
#include <data/string-decorator.h>
#include <data/entity-collection.h>

namespace cafe {
namespace data {

class CAFEAPPLIBSHARED_EXPORT Entity : public QObject
{
    Q_OBJECT

public:
    explicit Entity(QObject *parent = nullptr, const QString& key = "SomeKeyEntity");
    explicit Entity(QObject *parent, const QString& key, const QJsonObject& jsonObject );

    virtual ~Entity();
    const QString& key()const;
    const QString& id()const;

    QJsonObject toJson()const;
    void setPrimaryKey(StringDecorator* primaryKey);
    void update(const QJsonObject&);

signals:
    void childEntitiesChanged();
    void dataDecoratorsChanged();
    void childCollectionsChanged(const QString& childCollections);

protected:
    Entity *addChild(Entity *entity, const QString& key);
    DataDecorator *addDataItem(DataDecorator *dataDecorator);
    EntityCollectionBase *addChildCollection(EntityCollectionBase *entityCollection);
private:

    class Implementation;
    QScopedPointer<Implementation> implementation;
};

}
}
#endif // ENTITY_H
