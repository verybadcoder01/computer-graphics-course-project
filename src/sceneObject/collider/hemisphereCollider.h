#pragma once

#include "baseCollider.h"
#include "sceneObject/primitive/particle/particle.h"

class ThickHemisphereCollider: public Collider {
public:
    virtual ~ThickHemisphereCollider();

    bool checkCollision(Particle &p) const override;
protected:
    glm::vec3 center;
    glm::vec3 normal;
    float innerRadius;
    float outerRadius;
    float bounceCoeff;
};
