#include "particle.h"
#include <functional>
#include <random>
#include <thread>
#include "sceneObject/collider/baseCollider.h"

void Particle::update(float dt) {
    velocity += acceleration * dt;
    position += velocity * dt;
}

void Particle::decLife(float dt) noexcept {
    life -= dt;
}

bool Particle::isDead() const noexcept {
    return life <= 0;
}

void Particle::reset(const PositionVec &pos, const VelocityVec &vel, float nmaxLife) {
    position = pos;
    velocity = vel;
    maxLife = nmaxLife;
    life = maxLife;
}

ParticleSystem::ParticleSystem(size_t particleCount, const ParticleSystemParams &params)
    : particles(particleCount), params(params) {}

void ParticleSystem::killAll() noexcept {
    for (auto &p : particles) {
        p.kill();
    }
}

void ParticleSystem::update(float dt, const CollisionEnvironment &env) {
    if (particles.empty()) {
        return;
    }
    if (NUM_THREADS <= 1) {
        updateRange(0, particles.size(), dt, env);
        return;
    }

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

void ParticleSystem::updateRange(size_t start,
                                 size_t end,
                                 float dt,
                                 const CollisionEnvironment &env) {
    for (size_t i = start; i < end; ++i) {
        auto &p = particles[i];
        p.decLife(dt);
        if (p.isDead()) {
            resetParticle(p);
            continue;
        }

        p.update(dt);
        for (const auto &collider : env) {
            collider->checkCollision(p);
        }
    }
}

void ParticleSystem::resetParticle(Particle &p) {
    thread_local std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> distVel(params.minVel, params.maxVel);
    std::uniform_real_distribution<float> distLife(params.minLife, params.maxLife);
    VelocityVec localVelocity{distVel(rng) + params.startVx, distVel(rng) + params.startVy,
                              distVel(rng) + params.startVz};
    p.reset(origin, spawnRotation * localVelocity, distLife(rng));
}