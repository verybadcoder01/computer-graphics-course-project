#include "sphereCollider.h"
#include <glm/geometric.hpp>
#include "sceneObject/primitive/particle/particle.h"

bool SphereCollider::checkCollision(Particle &p) const {
    glm::vec3 diff = p.getPosition() - center;
    float distSq = glm::dot(diff, diff);
    float collisionDist = p.getRadius() + radius;
    if (distSq <= collisionDist * collisionDist) {
        glm::vec3 normal = glm::normalize(diff);
        p.setVelocity(glm::reflect(p.getVelocity(), normal) * bounceCoeff);
        p.setPosition(center + normal * collisionDist);
        return true;
    }
    return false;
}
