#include "editModeState.h"
#include "visitor/particleResetVisitor/particleResetVisitor.h"

void EditModeState::onEnter(EditorContext &ctx) {
    ctx.simulationPaused = false;
    ParticleResetVisitor reset;
    ctx.scene->accept(reset);
}
