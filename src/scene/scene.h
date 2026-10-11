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

    const KeyType &addObject(const ObjectType &obj);
    const ObjectType &getObject(const KeyType &key) const;
    void removeObject(const KeyType &key);

    void update(float dt);
    void accept(ObjectVisitor &visitor);
    void clear();

   protected:
    std::map<KeyType, ObjectType> objects{};
    KeyType curKey = 0;

    Scene() = default;
    void genNextKey() noexcept;
};
