#pragma once
#include <memory>
#include <string>
#include "editor/command/commandInterface.h"
#include "editor/selectionManager/selectionManager.h"
#include "renderPipeline/basePipeline.h"
#include "scene/scene.h"
#include "sceneObject/primitive/camera/camera.h"

enum class GizmoOperation { Translate, Rotate, Scale };
enum class GizmoMode { Local, World };
enum class EditorMode { Edit, Simulation };

struct EditorContext {
    std::shared_ptr<Scene> scene;
    std::shared_ptr<BasePipeline> pipeline;
    std::shared_ptr<SelectionManager> selection;
    std::shared_ptr<CommandHistory> history;
    std::shared_ptr<Camera> camera;

    GizmoOperation currentGizmoOp = GizmoOperation::Translate;
    GizmoMode currentGizmoMode = GizmoMode::World;

    EditorMode mode = EditorMode::Edit;
    bool simulationPaused = false;
    bool viewportHovered = false;
    std::string statusMessage;
};
