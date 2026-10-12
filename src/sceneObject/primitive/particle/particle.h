#pragma once

#include <type_traits>
#include <vector>
#include "glm/glm.hpp"
#include "sceneObject/collider/collisionEnvironment.h"

static constexpr double G_norm = -9.81;

typedef glm::vec3 PositionVec;
typedef glm::vec3 VelocityVec;
typedef glm::vec3 AccelerationVec;

class Particle {
   public:
    Particle() = default;
    ~Particle() = default;

    static constexpr size_t getPositionOffset() noexcept { return offsetof(Particle, position); }
    static constexpr size_t getLifeOffset() noexcept { return offsetof(Particle, life); }
    static constexpr size_t getRadiusOffset() noexcept { return offsetof(Particle, radius); }

    void update(float dt);
    void decLife(float dt) noexcept;
    bool isDead() const noexcept;
    void kill() noexcept { life = 0; }
    void reset(const PositionVec &pos, const VelocityVec &vel, float nmaxLife);

    PositionVec getPosition() const noexcept { return position; }
    VelocityVec getVelocity() const noexcept { return velocity; }
    float getRadius() const noexcept { return radius; }
    void setPosition(const PositionVec &p) noexcept { position = p; }
    void setVelocity(const VelocityVec &v) noexcept { velocity = v; }

   protected:
    PositionVec position{0};
    VelocityVec velocity{0};
    AccelerationVec acceleration{0, G_norm, 0};
    float life = 0, maxLife = 2;
    float radius = 0.1;
};
static_assert(std::is_standard_layout<Particle>::value);

constexpr int NUM_THREADS = 4;

struct ParticleSystemParams {
    float minVel = -2;
    float maxVel = 2;
    float minLife = 1;
    float maxLife = 3;
    float startVx = 0;
    float startVy = 5;
    float startVz = 0;
};

class ParticleSystem {
   public:
    ParticleSystem(size_t particleCount, const ParticleSystemParams &params);
    ~ParticleSystem() = default;

    void update(float dt, const CollisionEnvironment &env);
    void setOrigin(const PositionVec &newOrigin) noexcept { origin = newOrigin; }
    void setSpawnRotation(const glm::mat3 &rotation) noexcept { spawnRotation = rotation; }
    const std::vector<Particle> &getParticles() const { return particles; }
    const ParticleSystemParams &getParams() const noexcept { return params; }
    void setParams(const ParticleSystemParams &newParams) noexcept { params = newParams; }
    void setParticleCount(size_t count) { particles.resize(count); }
    void killAll() noexcept;

    size_t getParticlesDataSizeBytes() const noexcept {
        return particles.size() * sizeof(Particle);
    }
    const void *getRawParticlesData() const { return particles.data(); }
    size_t getParticleStride() const { return sizeof(Particle); }

   private:
    std::vector<Particle> particles;
    ParticleSystemParams params;
    PositionVec origin{0};
    glm::mat3 spawnRotation{1.0f};

    void updateRange(size_t start, size_t end, float dt, const CollisionEnvironment &env);
    void resetParticle(Particle &p);
};
