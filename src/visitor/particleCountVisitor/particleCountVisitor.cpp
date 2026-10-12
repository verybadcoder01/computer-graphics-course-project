#include "particleCountVisitor.h"
#include "sceneObject/primitive/particleEmitter/particleEmitter.h"

void ParticleCountVisitor::visit(ParticleEmitter &emitter) {
    count += emitter.getParticleCount();
}
