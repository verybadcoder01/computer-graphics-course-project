#include "simulationModeState.h"
#include "visitor/particleResetVisitor/particleResetVisitor.h"

void SimulationModeState::onEnter(EditorContext &ctx) {
    ctx.simulationPaused = false;
    ParticleResetVisitor reset;
    ctx.scene->accept(reset);
}

void SimulationModeState::onExit(EditorContext &ctx) {
    ctx.simulationPaused = false;
}

void SimulationModeState::update(EditorContext &ctx, float dt) {
    if (!ctx.simulationPaused) {
        ctx.scene->update(dt);
    }
}
