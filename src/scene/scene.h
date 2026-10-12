#pragma once

#include <map>
#include "sceneObject/baseObject.h"
#include "visitor/objectVisitor.h"

class Scene {
   public:
    using KeyType = uint32_t;
    using ObjectType = std::shared_ptr<BaseObject>;

    static std::shared_ptr<Scene> getInstance();
    virtual ~Scene() = default;

    KeyType addObject(const ObjectType &obj);
    const ObjectType &getObject(const KeyType &key) const;
    void removeObject(const KeyType &key);
    bool contains(const KeyType &key) const { return objects.contains(key); }
    const std::map<KeyType, ObjectType> &getObjects() const noexcept { return objects; }
    // call this before the GL context is destroyed
    void clearObjects() noexcept { objects.clear(); }

    void update(float dt);
    void accept(ObjectVisitor &visitor);

   protected:
    std::map<KeyType, ObjectType> objects{};

    Scene() = default;
};
