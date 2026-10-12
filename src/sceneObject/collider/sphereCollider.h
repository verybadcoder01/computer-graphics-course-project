#pragma once

#include "baseCollider.h"
#include "sceneObject/primitive/particle/particle.h"

class SphereCollider : public Collider {
   public:
    SphereCollider(const Model &model, float bounceCoeff);
    virtual ~SphereCollider() override = default;

    void updateTransform(const glm::mat4 &worldMatrix) override;
    bool checkCollision(Particle &p) const override;
    ColliderType getType() const noexcept override { return ColliderType::Sphere; }

   protected:
    glm::vec3 localCenter{0};
    float localRadius = 0;

    glm::vec3 center{0};
    float radius = 0;
};
