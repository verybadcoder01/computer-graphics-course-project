#pragma once

#include "baseCollider.h"
#include "sceneObject/primitive/particle/particle.h"

class SphereCollider : public Collider {
public:
    virtual ~SphereCollider() = 0;

    bool checkCollision(Particle &p) const;
protected:
    glm::vec3 center;
    float radius;
    float bounceCoeff;
};