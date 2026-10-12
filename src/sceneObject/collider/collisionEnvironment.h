#pragma once

#include <memory>
#include <vector>

class Collider;

using CollisionEnvironment = std::vector<std::shared_ptr<Collider>>;
