#include "sceneObject/primitive/particleEmitter/particleEmitter.h"
#include <memory>
#include "sceneObject/primitive/particle/particle.h"
#include "visitor/objectVisitor.h"

ParticleEmitter::ParticleEmitter(const ParticleSystemParams &params,
                                 std::shared_ptr<Model> visualMesh)
    : visualMesh(std::move(visualMesh)) {
    particles = std::make_unique<ParticleSystem>(params);
}

void ParticleEmitter::linkVisualMesh() {
    if (visualMesh && !visualMesh->getParent()) {
        visualMesh->setParent(weak_from_this());
    }
}

void ParticleEmitter::accept(ObjectVisitor &visitor) {
    linkVisualMesh();
    if (visualMesh) {
        visualMesh->accept(visitor);
    }
    visitor.visit(*this);
}

void ParticleEmitter::update(const SimulationContext &ctx) {
    static const CollisionEnvironment noColliders{};

    linkVisualMesh();
    const glm::mat4 world = getWorldMatrix();
    particles->setOrigin(glm::vec3(world[3]));
    particles->setSpawnRotation(glm::mat3(glm::normalize(glm::vec3(world[0])),
                                          glm::normalize(glm::vec3(world[1])),
                                          glm::normalize(glm::vec3(world[2]))));
    particles->update(ctx.dt, ctx.collisionEnv ? *ctx.collisionEnv : noColliders);
}
