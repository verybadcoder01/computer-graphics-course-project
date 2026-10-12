#pragma once

#include "editor/editorContext.h"

class IEditorModeState {
   public:
    virtual ~IEditorModeState() = default;

    virtual EditorMode getMode() const noexcept = 0;
    virtual void onEnter(EditorContext &ctx) = 0;
    virtual void onExit(EditorContext &ctx) = 0;
    virtual void update(EditorContext &ctx, float dt) = 0;
    virtual bool allowsEditing() const noexcept = 0;
};
