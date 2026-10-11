#pragma once

#include "sceneObject/collider/baseCollider.h"
#include "sceneObject/primitive/particle/particle.h"

class BoxCollider: public Collider {
public:
    virtual ~BoxCollider();

    bool checkCollision(Particle &p) const;

protected:
    glm::vec3 minBounds{0};
    glm::vec3 maxBounds{0};
    float bounceCoeff;
};
