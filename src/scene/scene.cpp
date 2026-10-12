#include "scene.h"
#include <memory>
#include <source_location>
#include "exceptions/scene/sceneExceptions.h"
#include "sceneObject/simulationContext.h"
#include "visitor/collisionCollectorVisitor/collisionCollectorVisitor.h"

std::shared_ptr<Scene> Scene::getInstance() {
    class Proxy : public Scene {};
    static std::shared_ptr<Scene> instance = std::make_shared<Proxy>();
    return instance;
}

Scene::KeyType Scene::addObject(const ObjectType &obj) {
    if (!obj) {
        throw InvalidSceneObjectException("Cannot add a null object to the scene",
                                          std::source_location::current());
    }
    objects[obj->getId()] = obj;
    return obj->getId();
}

const Scene::ObjectType &Scene::getObject(const KeyType &key) const {
    if (!objects.contains(key)) {
        throw ObjectKeyNotFoundException(std::format("Key {} not found", key),
                                         std::source_location::current());
    }
    return objects.at(key);
}

void Scene::removeObject(const KeyType &key) {
    if (!objects.contains(key)) {
        throw ObjectKeyNotFoundException(std::format("Key {} not found", key),
                                         std::source_location::current());
    }
    objects.erase(key);
}

void Scene::update(float dt) {
    CollisionCollectorVisitor collector;
    accept(collector);

    SimulationContext ctx{
        .dt = dt,
        .collisionEnv = std::make_shared<const CollisionEnvironment>(collector.getColliders())};
    for (const auto &[_, obj] : objects) {
        obj->update(ctx);
    }
}

void Scene::accept(ObjectVisitor &visitor) {
    for (const auto &[_, obj] : objects) {
        obj->accept(visitor);
    }
}
