#pragma once

#include <memory>
#include "sceneObject/collider/collisionEnvironment.h"
#include "sceneObject/primitive/particle/particle.h"

class Model;

enum class ColliderType { Sphere = 0, Box, Bowl };

class Collider {
   public:
    explicit Collider(float bounceCoeff) : bounceCoeff(bounceCoeff) {}
    virtual ~Collider() = default;

    virtual ColliderType getType() const noexcept = 0;

    virtual void updateTransform(const glm::mat4 &worldMatrix) = 0;
    virtual bool checkCollision(Particle &p) const = 0;

    float getBounceCoeff() const noexcept { return bounceCoeff; }
    void setBounceCoeff(float value) noexcept { bounceCoeff = value; }

   protected:
    float bounceCoeff;

    void resolve(Particle &p, const glm::vec3 &newPos, const glm::vec3 &normal) const;

    static std::vector<glm::vec3> getVertexPositions(const Model &model);
    static void computeBounds(const std::vector<glm::vec3> &points,
                              glm::vec3 &minBounds,
                              glm::vec3 &maxBounds);
    static float getMaxAxisScale(const glm::mat4 &matrix);
};
