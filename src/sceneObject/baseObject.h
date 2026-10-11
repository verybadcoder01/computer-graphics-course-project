#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include "glm/glm.hpp"
#include "visitor/objectVisitor.h"

class BaseObject : public std::enable_shared_from_this<BaseObject> {
   public:
    virtual ~BaseObject() = default;
    virtual void accept(ObjectVisitor &visitor) = 0;
    virtual void update(float dt) = 0;

    glm::mat4 getModelMatrix() const;
    glm::mat4 getWorldMatrix() const;
    virtual bool isVisible() const { return visible; };
    uint32_t getId() const noexcept { return id; }

    const glm::vec4 &getPosition() const noexcept { return position; }
    void setPosition(const glm::vec3 &pos) { position = glm::vec4(pos, 1.0); }

   protected:
    uint32_t id;
    std::string name;
    bool visible;
    glm::vec4 position{0, 0, 0, 1};
    glm::vec3 rotation{0, 0, 0}, scale{1, 1, 1};
    std::weak_ptr<BaseObject> parent;
};