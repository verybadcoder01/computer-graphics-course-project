#pragma once

#include "sceneObject/primitive/particle/particle.h"
#include <memory>

class Collider {
    virtual ~Collider() = 0;
    virtual bool checkCollision(Particle &p) const = 0;
};

using CollisionEnvironment = std::vector<std::shared_ptr<Collider>>;