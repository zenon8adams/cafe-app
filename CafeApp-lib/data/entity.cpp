#include <QUuid>
#include "entity.h"

namespace cafe {
namespace data {

class Entity::Implementation
{
public:
    Implementation(Entity *_entity, const QString& _key)
                  : entity(_entity), key(_key), id(QUuid::createUuid().toString())
    {
    }
    Entity *entity;
    QString key, id;
    StringDecorator *primaryKey{ nullptr };
    std::map<QString, Entity *> childEntities;
    std::map<QString, DataDecorator *> dataDecorators;
    std::map<QString, EntityCollectionBase *> childCollections;
};

Entity::Entity(QObject *parent, const QString& key) : QObject(parent)
{
    implementation.reset(new Implementation(this, key));
}

Entity::Entity(QObject *parent, const QString &key, const QJsonObject &jsonObject)
              : Entity(parent, key)
{
    update(jsonObject);
}

Entity::~Entity()
{
}

const QString& Entity::key()const
{
    return implementation->key;
}
const QString& Entity::id()const
{
    if(implementation->primaryKey != nullptr && !implementation->primaryKey->value().isEmpty())
        return implementation->primaryKey->value();
    return implementation->id;
}
Entity *Entity::addChild(Entity *entity, const QString &key)
{
    if(implementation->childEntities.find(key) == std::end(implementation->childEntities))
    {
        implementation->childEntities[key] = entity;
        emit childEntitiesChanged();
    }
    return entity;
}
DataDecorator *Entity::addDataItem(DataDecorator *dataDecorator)
{
    if(implementation->dataDecorators.find(dataDecorator->key()) == std::end(implementation->dataDecorators))
    {
        implementation->dataDecorators[dataDecorator->key()] = dataDecorator;
        emit dataDecoratorsChanged();
    }
    return dataDecorator;
}

void Entity::update(const QJsonObject& jsonObject)
{
    if(jsonObject.contains("id"))
        implementation->id = jsonObject.value("id").toString();

    for(auto& dataDecoratorPair : implementation->dataDecorators)
        dataDecoratorPair.second->update(jsonObject);
    for(auto& childEntityPair : implementation->childEntities)
        childEntityPair.second->update(jsonObject.value(childEntityPair.first).toObject());
    for(auto& childCollectionPair : implementation->childCollections)
        childCollectionPair.second->update(jsonObject.value(childCollectionPair.first).toArray());

}

QJsonObject Entity::toJson()const
{
    QJsonObject returnValue;

    returnValue.insert("id", implementation->id);
    for(const auto& dataDecoratorPair : implementation->dataDecorators)
        returnValue.insert(dataDecoratorPair.first, dataDecoratorPair.second->jsonValue());
    for(const auto& childEntityPair : implementation->childEntities)
        returnValue.insert(childEntityPair.first, childEntityPair.second->toJson());
    for(const auto& childCollectionPair : implementation->childCollections)
    {
        QJsonArray entityArray;
        for(auto entity : childCollectionPair.second->baseEntities())
            entityArray.append(entity->toJson());
        returnValue.insert(childCollectionPair.first, entityArray);
    }
    return returnValue;
}

EntityCollectionBase *Entity::addChildCollection(EntityCollectionBase *entityCollection)
{
    if(implementation->childCollections.find(entityCollection->getKey()) == std::end(implementation->childCollections))
    {
        implementation->childCollections[entityCollection->getKey()] = entityCollection;
        emit childCollectionsChanged(entityCollection->getKey());
    }
    return entityCollection;
}
void Entity::setPrimaryKey(StringDecorator* primaryKey)
{
    implementation->primaryKey = primaryKey;
}
}
}
