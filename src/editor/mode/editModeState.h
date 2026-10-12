#pragma once

#include "editor/mode/editorModeState.h"

// scene is static and editable; entering clears all particles
class EditModeState : public IEditorModeState {
   public:
    EditorMode getMode() const noexcept override { return EditorMode::Edit; }
    void onEnter(EditorContext &ctx) override;
    void onExit(EditorContext &) override {}
    void update(EditorContext &, float) override {}
    bool allowsEditing() const noexcept override { return true; }
};
