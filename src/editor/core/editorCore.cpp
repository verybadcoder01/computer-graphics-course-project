#include "editorCore.h"

std::shared_ptr<EditorCore> EditorCore::create(
    std::shared_ptr<Scene> scene,
    std::shared_ptr<BasePipeline> pipeline,
    std::shared_ptr<ObjectPickingPass> pickingPass
) {
    return std::shared_ptr<EditorCore>(new EditorCore(
        std::move(scene), 
        std::move(pipeline), 
        std::move(pickingPass)
    ));
}

EditorCore::EditorCore(
    std::shared_ptr<Scene> scene,
    std::shared_ptr<BasePipeline> pipeline,
    std::shared_ptr<ObjectPickingPass> pickingPass
) : pickingPass(std::move(pickingPass)) {
    
    context = std::make_shared<EditorContext>();
    context->scene = std::move(scene);
    context->pipeline = std::move(pipeline);
    context->selection = std::make_shared<SelectionManager>();
    context->history = std::make_shared<CommandHistory>();
}

void EditorCore::update(float dt) {
    if (context->isSimulating && context->scene) {
        context->scene->update(dt);
    }
}

void EditorCore::handleViewportClick(int mouseX, int mouseY) {
    if (!pickingPass || !context->scene) return;

    uint32_t objectID = pickingPass->readObjectAt(mouseX, mouseY);
    context->selection->selectByID(context->scene, objectID);
}

void EditorCore::executeCommand(std::unique_ptr<ICommand> cmd) {
    if (context->history) {
        context->history->executeCommand(std::move(cmd));
    }
}

void EditorCore::undo() {
    if (context->history) {
        context->history->undo();
    }
}

void EditorCore::redo() {
    if (context->history) {
        context->history->redo();
    }
}

void EditorCore::setSimulationState(bool running) noexcept {
    context->isSimulating = running;
}

bool EditorCore::isSimulating() const noexcept {
    return context->isSimulating;
}
