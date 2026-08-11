#include "particle.h"
#include <random>
#include <thread>

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
  : particles(particleCount), rd(), params(params) {
    for (auto &p : particles){ 
        resetParticle(p);
    }
}

void ParticleSystem::update(float dt) {
    if (particles.empty()) {
        return;
    }
    if (NUM_THREADS <= 1) {
        updateRange(0, particles.size(), dt);
        return;
    }

    std::vector<std::thread> workers;
    workers.reserve(NUM_THREADS);
    size_t chunkSize = particles.size() / NUM_THREADS;
    
    for (size_t i = 0; i < NUM_THREADS; ++i){
        size_t startIndex = i * chunkSize;
        size_t endIndex = startIndex + chunkSize;
        if (i == NUM_THREADS - 1){
            endIndex = particles.size();
        }
        workers.emplace_back(&ParticleSystem::updateRange, this, startIndex, endIndex, dt);
    }

    for (auto &thread: workers){
        if (thread.joinable()){
            thread.join();
        }
    }
}

void ParticleSystem::updateRange(size_t start, size_t end, float dt) {
    constexpr glm::vec3 gravity = {0, G_norm, 0};

    for (size_t i = start; i < end; ++i){
        auto &p = particles[i];
        p.decLife(dt);
        if (p.isDead()) {
            resetParticle(p);
            continue;
        }

        p.update(dt);
    }
}

void ParticleSystem::resetParticle(Particle &p) {
    thread_local std::mt19937 rng(rd());
    std::uniform_real_distribution<float> distVel(params.minVel, params.maxVel);
    std::uniform_real_distribution<float> distLife(params.minLife, params.maxLife);
    p.reset(PositionVec{0, 0, 0}, 
        VelocityVec{distVel(rng) + params.startVx, distVel(rng) + params.startVy, distVel(rng) + params.startVz},
         distLife(rng));
}