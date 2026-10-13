#include "particle.h"
#include <algorithm>
#include <cmath>
#include <functional>
#include <glm/trigonometric.hpp>
#include <random>
#include <thread>
#include "sceneObject/collider/baseCollider.h"

namespace {

std::mt19937 &randomEngine() {
    thread_local std::mt19937 rng(std::random_device{}());
    return rng;
}

float uniform(float a, float b) {
    return std::uniform_real_distribution<float>(std::min(a, b), std::max(a, b))(randomEngine());
}

}  // namespace

void Particle::update(float dt, float drag) {
    velocity += acceleration * dt;
    velocity *= std::max(0.0f, 1.0f - drag * dt);
    position += velocity * dt;
}

void Particle::decLife(float dt) noexcept {
    life -= dt;
}

bool Particle::isDead() const noexcept {
    return life <= 0;
}

void Particle::reset(const PositionVec &pos, const VelocityVec &vel, float nmaxLife, float nrad) {
    position = pos;
    velocity = vel;
    maxLife = nmaxLife;
    life = maxLife;
    radius = nrad;
}

ParticleSystem::ParticleSystem(const ParticleSystemParams &params) : params(params) {}

void ParticleSystem::killAll() noexcept {
    particles.clear();
    spawnAccumulator = 0;
}

void ParticleSystem::update(float dt, const CollisionEnvironment &env) {
    if (NUM_THREADS <= 1 || particles.size() < NUM_THREADS) {
        updateRange(0, particles.size(), dt, env);
    } else {
        std::vector<std::thread> workers;
        workers.reserve(NUM_THREADS);
        size_t chunkSize = particles.size() / NUM_THREADS;

        for (size_t i = 0; i < NUM_THREADS; ++i) {
            size_t startIndex = i * chunkSize;
            size_t endIndex = startIndex + chunkSize;
            if (i == NUM_THREADS - 1) {
                endIndex = particles.size();
            }
            workers.emplace_back(&ParticleSystem::updateRange, this, startIndex, endIndex, dt,
                                 std::cref(env));
        }

        for (auto &thread : workers) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

    removeDead();
    spawn(dt);
}

void ParticleSystem::updateRange(size_t start,
                                 size_t end,
                                 float dt,
                                 const CollisionEnvironment &env) {
    for (size_t i = start; i < end; ++i) {
        auto &p = particles[i];
        p.decLife(dt);
        if (p.isDead()) {
            continue;
        }

        float remaining = dt;
        for (int step = 0; step < MAX_SUBSTEPS && remaining > 0.0f; ++step) {
            const float speed = glm::length(p.getVelocity());
            float h = remaining;
            if (step < MAX_SUBSTEPS - 1 && speed * h > MAX_SUBSTEP_DISTANCE) {
                h = MAX_SUBSTEP_DISTANCE / speed;
            }
            p.update(h, params.drag);
            for (const auto &collider : env) {
                collider->checkCollision(p);
            }
            remaining -= h;
        }
        if (p.getPosition().y < DEATH_ZONE_Y) {
            p.kill();
        }
    }
}

void ParticleSystem::removeDead() {
    std::erase_if(particles, [](const Particle &p) { return p.isDead(); });
}

void ParticleSystem::spawn(float dt) {
    spawnAccumulator += std::max(params.emissionRate, 0.0f) * dt;
    const size_t count = static_cast<size_t>(spawnAccumulator);
    spawnAccumulator -= static_cast<float>(count);

    particles.reserve(particles.size() + count);
    for (size_t i = 0; i < count; ++i) {
        Particle p;
        resetParticle(p);
        p.update(uniform(0.0f, dt), params.drag);
        particles.push_back(p);
    }
}

void ParticleSystem::resetParticle(Particle &p) {
    // uniform direction inside a cone of half-angle spreadAngle around the emitter's +Y
    const float cosSpread = std::cos(glm::radians(std::clamp(params.spreadAngle, 0.0f, 180.0f)));
    const float cosTheta = uniform(cosSpread, 1.0f);
    const float sinTheta = std::sqrt(std::max(0.0f, 1.0f - cosTheta * cosTheta));
    const float phi = uniform(0.0f, 2.0f * M_PI);
    const glm::vec3 direction{sinTheta * std::cos(phi), cosTheta, sinTheta * std::sin(phi)};

    const VelocityVec velocity =
        spawnRotation * (direction * uniform(params.minSpeed, params.maxSpeed));
    p.reset(origin, velocity, uniform(params.minLife, params.maxLife),
            uniform(params.minRad, params.maxRad));
}
