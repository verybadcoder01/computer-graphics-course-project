#pragma once

#include "editor/mode/editorModeState.h"

// scene is simulated (unless paused) and read-only; entering starts the particles fresh
class SimulationModeState : public IEditorModeState {
   public:
    EditorMode getMode() const noexcept override { return EditorMode::Simulation; }
    void onEnter(EditorContext &ctx) override;
    void onExit(EditorContext &ctx) override;
    void update(EditorContext &ctx, float dt) override;
    bool allowsEditing() const noexcept override { return false; }
};
