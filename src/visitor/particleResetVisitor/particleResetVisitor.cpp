#include "particleResetVisitor.h"
#include "sceneObject/primitive/particleEmitter/particleEmitter.h"

void ParticleResetVisitor::visit(ParticleEmitter &emitter) {
    emitter.resetParticles();
}
