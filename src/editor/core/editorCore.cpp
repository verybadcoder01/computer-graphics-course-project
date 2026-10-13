#include "editorCore.h"
#include "editor/command/commands/addObjectCommand.h"
#include "editor/command/commands/removeObjectCommand.h"
#include "editor/mode/editModeState.h"
#include "editor/mode/simulationModeState.h"
#include "exceptions/baseException.h"
#include "sceneObject/primitive/model/objModel/objModel.h"
#include "sceneObject/primitive/particleEmitter/particleEmitter.h"

std::shared_ptr<EditorCore> EditorCore::create(std::shared_ptr<Scene> scene,
                                               std::shared_ptr<BasePipeline> pipeline,
                                               std::shared_ptr<ObjectPickingPass> pickingPass,
                                               std::shared_ptr<Camera> camera) {
    return std::shared_ptr<EditorCore>(new EditorCore(std::move(scene), std::move(pipeline),
                                                      std::move(pickingPass), std::move(camera)));
}

EditorCore::EditorCore(std::shared_ptr<Scene> scene,
                       std::shared_ptr<BasePipeline> pipeline,
                       std::shared_ptr<ObjectPickingPass> pickingPass,
                       std::shared_ptr<Camera> camera)
    : context(std::make_shared<EditorContext>()),
      viewport(pipeline, std::move(pickingPass), camera),
      cameraController(camera) {
    context->scene = std::move(scene);
    context->pipeline = std::move(pipeline);
    context->selection = std::make_shared<SelectionManager>();
    context->history = std::make_shared<CommandHistory>();
    context->camera = std::move(camera);

    modeState = std::make_unique<EditModeState>();
    context->mode = modeState->getMode();
    modeState->onEnter(*context);
}

void EditorCore::update(float dt, const InputState &input) {
    lastFrameTime = dt;
    handleShortcuts(input);
    cameraController.update(dt, input, context->viewportHovered);
    modeState->update(*context, dt);
}

void EditorCore::renderViewport() {
    if (auto picked = viewport.render(*context->scene)) {
        context->selection->selectByID(context->scene, *picked);
    }
}

void EditorCore::setMode(EditorMode mode) {
    if (mode == context->mode) {
        return;
    }
    modeState->onExit(*context);
    if (mode == EditorMode::Simulation) {
        modeState = std::make_unique<SimulationModeState>();
    } else {
        modeState = std::make_unique<EditModeState>();
    }
    context->mode = mode;
    modeState->onEnter(*context);
}

void EditorCore::setPaused(bool paused) noexcept {
    if (context->mode == EditorMode::Simulation) {
        context->simulationPaused = paused;
    }
}

void EditorCore::select(std::shared_ptr<BaseObject> object) {
    context->selection->select(std::move(object));
}

bool EditorCore::executeCommand(std::unique_ptr<ICommand> cmd) {
    if (!cmd || !canEdit()) {
        return false;
    }
    try {
        context->history->executeCommand(std::move(cmd));
    } catch (const BaseException &e) {
        setStatusMessage(e.what());
        return false;
    }
    context->statusMessage.clear();
    return true;
}

void EditorCore::undo() {
    if (canEdit()) {
        context->history->undo();
    }
}

void EditorCore::redo() {
    if (canEdit()) {
        context->history->redo();
    }
}

void EditorCore::spawnFromLibrary(const LibraryEntry &entry) {
    if (!canEdit()) {
        return;
    }
    try {
        auto object = createObject(entry);
        object->setPosition(computeSpawnPosition());
        executeCommand(
            std::make_unique<AddObjectCommand>(context->scene, context->selection, object));
    } catch (const BaseException &e) {
        setStatusMessage(e.what());
    }
}

void EditorCore::removeSelected() {
    auto selected = getSelected();
    if (!isRemovable(selected)) {
        return;
    }
    executeCommand(
        std::make_unique<RemoveObjectCommand>(context->scene, context->selection, selected));
}

bool EditorCore::isRemovable(const std::shared_ptr<BaseObject> &object) const {
    return object && object != context->camera && canEdit();
}

std::shared_ptr<BaseObject> EditorCore::createObject(const LibraryEntry &entry) const {
    auto model = std::make_shared<ObjModel>(entry.name, entry.objPath, entry.texturePath);
    if (entry.kind == LibraryEntryKind::Part) {
        return model;
    }

    auto emitter = std::make_shared<ParticleEmitter>(ParticleSystemParams{}, std::move(model));
    emitter->setName(entry.name);
    return emitter;
}

glm::vec3 EditorCore::computeSpawnPosition() const {
    const glm::vec3 origin = glm::vec3(context->camera->getPosition());
    const glm::vec3 front = context->camera->getFront();

    // where the view ray hits the ground plane y = 0, if it does so reasonably close
    if (front.y < -1e-3f) {
        float t = -origin.y / front.y;
        if (t > 0.0f && t < maxGroundDistance) {
            return origin + front * t;
        }
    }
    return origin + front * spawnDistance;
}

void EditorCore::handleShortcuts(const InputState &input) {
    if (input.wantsTextInput) {
        return;
    }
    if (input.ctrl && input.isPressed(Key::Z)) {
        input.shift ? redo() : undo();
    } else if (input.ctrl && input.isPressed(Key::Y)) {
        redo();
    }
    if (input.isPressed(Key::Delete)) {
        removeSelected();
    }
    if (input.isPressed(Key::F5)) {
        setMode(getMode() == EditorMode::Edit ? EditorMode::Simulation : EditorMode::Edit);
    }
    if (input.isPressed(Key::Space) && getMode() == EditorMode::Simulation) {
        setPaused(!isPaused());
    }
}
