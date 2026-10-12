#pragma once

#include <array>
#include "sceneObject/collider/baseCollider.h"
#include "sceneObject/primitive/particle/particle.h"

class BoxCollider : public Collider {
   public:
    BoxCollider(const Model &model, float bounceCoeff);
    virtual ~BoxCollider() override = default;

    void updateTransform(const glm::mat4 &worldMatrix) override;
    bool checkCollision(Particle &p) const override;
    ColliderType getType() const noexcept override { return ColliderType::Box; }

   protected:
    glm::vec3 minBounds{0};
    glm::vec3 maxBounds{0};

    glm::vec3 center{0};
    std::array<glm::vec3, 3> axes{glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)};
    glm::vec3 halfExtents{0};
};
