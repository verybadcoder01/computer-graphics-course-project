#pragma once

#include <memory>
#include "sceneObject/collider/collisionEnvironment.h"

struct SimulationContext {
    float dt = 0;
    std::shared_ptr<const CollisionEnvironment> collisionEnv = nullptr;
};
