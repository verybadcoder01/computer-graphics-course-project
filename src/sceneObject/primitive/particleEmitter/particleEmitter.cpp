#include "sceneObject/primitive/particleEmitter/particleEmitter.h"
#include <memory>
#include "sceneObject/primitive/particle/particle.h"
#include "visitor/objectVisitor.h"

ParticleEmitter::ParticleEmitter(size_t particleCount,
                                 const ParticleSystemParams &params,
                                 std::shared_ptr<Model> visualMesh)
    : visualMesh(std::move(visualMesh)) {
    particles = std::make_unique<ParticleSystem>(particleCount, params);
}

void ParticleEmitter::accept(ObjectVisitor &visitor) {
    if (visualMesh) {
        visualMesh->setPosition(position);
        visualMesh->accept(visitor);
    }
    visitor.visit(*this);
}

void ParticleEmitter::update(float dt) {
    particles->update(dt);
}
