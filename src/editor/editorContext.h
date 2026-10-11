#pragma once
#include "scene/scene.h"
#include "renderPipeline/basePipeline.h"
#include "editor/selectionManager/selectionManager.h"
#include "editor/command/comamndInterface.h"
#include <memory>

enum class GizmoOperation { Translate, Rotate, Scale };
enum class GizmoMode { Local, World };

struct EditorContext {
    std::shared_ptr<Scene> scene;
    std::shared_ptr<BasePipeline> pipeline;
    std::shared_ptr<SelectionManager> selection;
    std::shared_ptr<CommandHistory> history;

    GizmoOperation currentGizmoOp = GizmoOperation::Translate;
    GizmoMode currentGizmoMode    = GizmoMode::World;

    bool isSimulating = false;
    bool isViewportHovered = false;
    bool isViewportFocused = false;
};
