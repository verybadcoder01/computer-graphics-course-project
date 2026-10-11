#include "scene.h"
#include <memory>
#include <source_location>
#include "exceptions/scene/sceneExceptions.h"
#include "glad/glad.h"

std::shared_ptr<Scene> Scene::getInstance() {
    class Proxy : public Scene {};
    static std::shared_ptr<Scene> instance = std::make_shared<Proxy>();
    return instance;
}

void Scene::genNextKey() noexcept {
    curKey++;
}

const Scene::KeyType &Scene::addObject(const ObjectType &obj) {
    genNextKey();
    objects[curKey] = obj;
    return curKey;
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
    for (const auto &[_, obj] : objects) {
        obj->update(dt);
    }
}

void Scene::accept(ObjectVisitor &visitor) {
    for (const auto &[_, obj] : objects) {
        obj->accept(visitor);
    }
}

void Scene::clear() {
    glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
