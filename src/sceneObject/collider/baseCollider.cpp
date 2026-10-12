#include "baseCollider.h"
#include <algorithm>
#include <limits>
#include <source_location>
#include "exceptions/collider/colliderExceptions.h"
#include "sceneObject/primitive/model/model.h"

void Collider::resolve(Particle &p, const glm::vec3 &newPos, const glm::vec3 &normal) const {
    p.setPosition(newPos);

    glm::vec3 vel = p.getVelocity();
    float normalSpeed = glm::dot(vel, normal);
    if (normalSpeed < 0.0f) {
        p.setVelocity(vel - (1.0f + bounceCoeff) * normalSpeed * normal);
    }
}

std::vector<glm::vec3> Collider::getVertexPositions(const Model &model) {
    const auto &vertices = model.getVertices();
    if (vertices.empty()) {
        throw InvalidColliderGeometryException(
            "Cannot build collider from a model without vertices", std::source_location::current());
    }

    std::vector<glm::vec3> positions;
    positions.reserve(vertices.size());
    for (const auto &vertex : vertices) {
        positions.push_back(vertex.position);
    }
    return positions;
}

void Collider::computeBounds(const std::vector<glm::vec3> &points,
                             glm::vec3 &minBounds,
                             glm::vec3 &maxBounds) {
    minBounds = glm::vec3(std::numeric_limits<float>::max());
    maxBounds = glm::vec3(std::numeric_limits<float>::lowest());
    for (const auto &point : points) {
        minBounds = glm::min(minBounds, point);
        maxBounds = glm::max(maxBounds, point);
    }
}

float Collider::getMaxAxisScale(const glm::mat4 &matrix) {
    return std::max({glm::length(glm::vec3(matrix[0])), glm::length(glm::vec3(matrix[1])),
                     glm::length(glm::vec3(matrix[2]))});
}
