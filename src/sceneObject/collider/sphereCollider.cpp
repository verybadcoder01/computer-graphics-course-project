#include "sphereCollider.h"
#include <algorithm>
#include <glm/geometric.hpp>
#include "sceneObject/primitive/particle/particle.h"

SphereCollider::SphereCollider(const Model &model, float bounceCoeff) : Collider(bounceCoeff) {
    const auto positions = getVertexPositions(model);
    glm::vec3 minBounds, maxBounds;
    computeBounds(positions, minBounds, maxBounds);

    localCenter = (minBounds + maxBounds) * 0.5f;
    for (const auto &pos : positions) {
        localRadius = std::max(localRadius, glm::length(pos - localCenter));
    }
    updateTransform(glm::mat4(1.0f));
}

void SphereCollider::updateTransform(const glm::mat4 &worldMatrix) {
    center = glm::vec3(worldMatrix * glm::vec4(localCenter, 1.0f));
    // non-uniform scale turns the sphere into an ellipsoid; approximate with the largest axis
    radius = localRadius * getMaxAxisScale(worldMatrix);
}

bool SphereCollider::checkCollision(Particle &p) const {
    glm::vec3 diff = p.getPosition() - center;
    float distSq = glm::dot(diff, diff);
    float collisionDist = p.getRadius() + radius;
    if (distSq > collisionDist * collisionDist) {
        return false;
    }

    const float eps = 1e-6f;
    glm::vec3 normal = distSq > eps ? diff / std::sqrt(distSq) : glm::vec3(0, 1, 0);
    resolve(p, center + normal * collisionDist, normal);
    return true;
}
