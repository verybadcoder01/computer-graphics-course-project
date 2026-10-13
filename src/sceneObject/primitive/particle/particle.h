#pragma once

#include <type_traits>
#include <vector>
#include "glm/glm.hpp"
#include "sceneObject/collider/collisionEnvironment.h"

static constexpr double G_norm = -9.81;
static constexpr float DEATH_ZONE_Y = -5.0f;
static constexpr float MAX_SUBSTEP_DISTANCE = 0.02f;
static constexpr int MAX_SUBSTEPS = 32;

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

    void update(float dt, float drag);
    void decLife(float dt) noexcept;
    bool isDead() const noexcept;
    void kill() noexcept { life = 0; }
    void reset(const PositionVec &pos, const VelocityVec &vel, float nmaxLife, float nrad);

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
    float emissionRate = 2000;  // particles per second
    float minSpeed = 5;
    float maxSpeed = 6;
    float spreadAngle = 8;  // degrees, half-angle of the spray cone around the emitter's +Y
    float minLife = 10;
    float maxLife = 15;
    float minRad = 0.001;
    float maxRad = 0.05;
    float drag = 0.1;  // 1/s
};

class ParticleSystem {
   public:
    explicit ParticleSystem(const ParticleSystemParams &params);
    ~ParticleSystem() = default;

    void update(float dt, const CollisionEnvironment &env);
    void setOrigin(const PositionVec &newOrigin) noexcept { origin = newOrigin; }
    void setSpawnRotation(const glm::mat3 &rotation) noexcept { spawnRotation = rotation; }
    const std::vector<Particle> &getParticles() const { return particles; }
    const ParticleSystemParams &getParams() const noexcept { return params; }
    void setParams(const ParticleSystemParams &newParams) noexcept { params = newParams; }
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
    float spawnAccumulator = 0;

    void updateRange(size_t start, size_t end, float dt, const CollisionEnvironment &env);
    void removeDead();
    void spawn(float dt);
    void resetParticle(Particle &p);
};
