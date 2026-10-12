#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include "glm/glm.hpp"
#include "sceneObject/simulationContext.h"
#include "visitor/objectVisitor.h"

class BaseObject : public std::enable_shared_from_this<BaseObject> {
   public:
    BaseObject() noexcept;
    BaseObject(const BaseObject &other);
    BaseObject &operator=(const BaseObject &other);
    virtual ~BaseObject() = default;

    virtual void accept(ObjectVisitor &visitor) = 0;
    virtual void update(const SimulationContext &ctx) = 0;

    glm::mat4 getModelMatrix() const;
    glm::mat4 getWorldMatrix() const;
    virtual bool isVisible() const { return visible; };

    uint32_t getId() const noexcept { return id; }
    uint32_t getRootId() const;

    const std::string &getName() const noexcept { return name; }
    void setName(const std::string &newName) { name = newName; }

    std::shared_ptr<BaseObject> getParent() const noexcept { return parent.lock(); }
    void setParent(const std::weak_ptr<BaseObject> &newParent) noexcept { parent = newParent; }

    const glm::vec4 &getPosition() const noexcept { return position; }
    void setPosition(const glm::vec3 &pos) { position = glm::vec4(pos, 1.0); }

    // euler angles in degrees, applied Z, then Y, then X
    const glm::vec3 &getRotation() const noexcept { return rotation; }
    void setRotation(const glm::vec3 &rot) noexcept { rotation = rot; }

    const glm::vec3 &getScale() const noexcept { return scale; }
    void setScale(const glm::vec3 &newScale) noexcept { scale = newScale; }

   protected:
    uint32_t id;
    std::string name;
    bool visible = true;
    glm::vec4 position{0, 0, 0, 1};
    glm::vec3 rotation{0, 0, 0}, scale{1, 1, 1};
    std::weak_ptr<BaseObject> parent;

   private:
    static uint32_t generateId() noexcept;
};