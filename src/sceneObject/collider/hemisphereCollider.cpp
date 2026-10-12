#include "hemisphereCollider.h"
#include <algorithm>
#include <format>
#include <limits>
#include <source_location>
#include "exceptions/collider/colliderExceptions.h"
#include "glm/matrix.hpp"
#include "sceneObject/primitive/particle/particle.h"

ThickHemisphereCollider::ThickHemisphereCollider(const Model &model, float bounceCoeff)
    : Collider(bounceCoeff) {
    const auto positions = getVertexPositions(model);
    glm::vec3 minBounds, maxBounds;
    computeBounds(positions, minBounds, maxBounds);

    localCenter = glm::vec3((minBounds.x + maxBounds.x) * 0.5f, maxBounds.y,
                            (minBounds.z + maxBounds.z) * 0.5f);
    localInnerRadius = std::numeric_limits<float>::max();
    for (const auto &pos : positions) {
        float dist = glm::length(pos - localCenter);
        localInnerRadius = std::min(localInnerRadius, dist);
        localOuterRadius = std::max(localOuterRadius, dist);
    }

    if (localInnerRadius >= localOuterRadius) {
        throw InvalidColliderGeometryException(
            std::format("Model is not a thick bowl: inner radius {} >= outer radius {}",
                        localInnerRadius, localOuterRadius),
            std::source_location::current());
    }
    updateTransform(glm::mat4(1.0f));
}

void ThickHemisphereCollider::updateTransform(const glm::mat4 &worldMatrix) {
    center = glm::vec3(worldMatrix * glm::vec4(localCenter, 1.0f));
    normal = glm::normalize(glm::transpose(glm::inverse(glm::mat3(worldMatrix))) * localNormal);
    float scale = getMaxAxisScale(worldMatrix);
    innerRadius = localInnerRadius * scale;
    outerRadius = localOuterRadius * scale;
}

bool ThickHemisphereCollider::checkCollision(Particle &p) const {
    const glm::vec3 pos = p.getPosition();
    const float particleRadius = p.getRadius();
    const glm::vec3 dir = pos - center;
    const float h = glm::dot(dir, normal);

    if (h < -particleRadius) {
        return false;
    }

    if (h > 0.0f) {
        float r = glm::length(dir);
        if (r < innerRadius - particleRadius || r > outerRadius + particleRadius || r < 1e-4f) {
            return false;
        }
        glm::vec3 unitDir = dir / r;

        if (r < innerRadius) {
            resolve(p, center + unitDir * (innerRadius - particleRadius), -unitDir);
        } else if (r > outerRadius) {
            resolve(p, center + unitDir * (outerRadius + particleRadius), unitDir);
        } else if (outerRadius - r < r - innerRadius) {
            resolve(p, center + unitDir * (outerRadius + particleRadius), unitDir);
        } else {
            resolve(p, center + unitDir * (innerRadius - particleRadius), -unitDir);
        }
        return true;
    }

    float projDist = glm::length(dir - normal * h);
    if (projDist >= innerRadius && projDist <= outerRadius) {
        resolve(p, pos - normal * (h + particleRadius), -normal);
        return true;
    }
    return false;
}
