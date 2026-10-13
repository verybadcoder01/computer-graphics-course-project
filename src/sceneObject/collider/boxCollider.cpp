#include "boxCollider.h"
#include <glm/geometric.hpp>
#include "sceneObject/primitive/particle/particle.h"

BoxCollider::BoxCollider(const Model &model, float bounceCoeff) : Collider(bounceCoeff) {
    computeBounds(getVertexPositions(model), minBounds, maxBounds);
    updateTransform(glm::mat4(1.0f));
}

void BoxCollider::updateTransform(const glm::mat4 &worldMatrix) {
    const glm::vec3 localCenter = (minBounds + maxBounds) * 0.5f;
    const glm::vec3 localHalfExtents = (maxBounds - minBounds) * 0.5f;
    center = glm::vec3(worldMatrix * glm::vec4(localCenter, 1.0f));

    const float eps = 1e-6f;
    for (int i = 0; i < 3; ++i) {
        glm::vec3 column = glm::vec3(worldMatrix[i]);
        float scale = glm::length(column);
        if (scale > eps) {
            axes[i] = column / scale;
        } else {
            axes[i] = glm::vec3(0);
            axes[i][i] = 1;
        }
        halfExtents[i] = localHalfExtents[i] * scale;
    }
}

bool BoxCollider::checkCollision(Particle &p) const {
    const glm::vec3 pos = p.getPosition();
    const float particleRadius = p.getRadius();
    const glm::vec3 diff = pos - center;

    // particle center in box coordinates
    glm::vec3 local, closest;
    bool inside = true;
    for (int i = 0; i < 3; ++i) {
        local[i] = glm::dot(diff, axes[i]);
        closest[i] = glm::clamp(local[i], -halfExtents[i], halfExtents[i]);
        if (closest[i] != local[i]) {
            inside = false;
        }
    }

    if (!inside) {
        glm::vec3 offset = local - closest;
        float distSq = glm::dot(offset, offset);
        if (distSq > particleRadius * particleRadius) {
            return false;
        }
        offset /= std::sqrt(distSq);

        glm::vec3 surfacePoint = center;
        glm::vec3 normal{0};
        for (int i = 0; i < 3; ++i) {
            surfacePoint += axes[i] * closest[i];
            normal += axes[i] * offset[i];
        }
        resolve(p, surfacePoint + normal * particleRadius, normal);
        return true;
    }

    const glm::vec3 velocity = p.getVelocity();
    int faceAxis = -1, nearestAxis = 0;
    float minTime = 0.0f, minDepth = 0.0f;
    for (int i = 0; i < 3; ++i) {
        const float side = local[i] >= 0.0f ? 1.0f : -1.0f;
        const float depth = halfExtents[i] - std::abs(local[i]);
        const float inwardSpeed = -side * glm::dot(velocity, axes[i]);
        if (i == 0 || depth < minDepth) {
            minDepth = depth;
            nearestAxis = i;
        }
        if (inwardSpeed > 1e-6f) {
            const float timeSinceEntry = depth / inwardSpeed;
            if (faceAxis < 0 || timeSinceEntry < minTime) {
                minTime = timeSinceEntry;
                faceAxis = i;
            }
        }
    }
    if (faceAxis < 0) {
        faceAxis = nearestAxis;
    }

    const float depth = halfExtents[faceAxis] - std::abs(local[faceAxis]);
    const glm::vec3 normal = axes[faceAxis] * (local[faceAxis] >= 0.0f ? 1.0f : -1.0f);
    resolve(p, pos + normal * (depth + particleRadius), normal);
    return true;
}
