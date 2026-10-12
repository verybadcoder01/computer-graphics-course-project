#pragma once

#include "baseCollider.h"
#include "sceneObject/primitive/particle/particle.h"

class ThickHemisphereCollider : public Collider {
   public:
    ThickHemisphereCollider(const Model &model, float bounceCoeff);
    virtual ~ThickHemisphereCollider() override = default;

    void updateTransform(const glm::mat4 &worldMatrix) override;
    bool checkCollision(Particle &p) const override;
    ColliderType getType() const noexcept override { return ColliderType::Bowl; }

   protected:
    glm::vec3 localCenter{0};
    const glm::vec3 localNormal{0, -1, 0};
    float localInnerRadius = 0;
    float localOuterRadius = 0;

    glm::vec3 center{0};
    glm::vec3 normal{0, -1, 0};
    float innerRadius = 0;
    float outerRadius = 0;
};
