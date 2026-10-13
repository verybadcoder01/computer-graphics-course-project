#pragma once
#include <memory>
#include <string>
#include "adapters/gui/guiAdapter.h"
#include "editor/camera/editorCameraController.h"
#include "editor/editorContext.h"
#include "editor/library/modelLibrary.h"
#include "editor/mode/editorModeState.h"
#include "editor/viewport/viewport.h"
#include "renderPipeline/renderPasses/objectPicking/objectPickingPass.h"

class EditorCore : public std::enable_shared_from_this<EditorCore> {
   public:
    static std::shared_ptr<EditorCore> create(std::shared_ptr<Scene> scene,
                                              std::shared_ptr<BasePipeline> pipeline,
                                              std::shared_ptr<ObjectPickingPass> pickingPass,
                                              std::shared_ptr<Camera> camera);

    void update(float dt, const InputState &input);
    void renderViewport();

    void setMode(EditorMode mode);
    EditorMode getMode() const noexcept { return context->mode; }
    void setPaused(bool paused) noexcept;
    bool isPaused() const noexcept { return context->simulationPaused; }
    bool canEdit() const noexcept { return modeState->allowsEditing(); }

    void select(std::shared_ptr<BaseObject> object);
    std::shared_ptr<BaseObject> getSelected() const { return context->selection->getSelected(); }

    bool executeCommand(std::unique_ptr<ICommand> cmd);
    void undo();
    void redo();
    bool canUndo() const noexcept { return canEdit() && context->history->canUndo(); }
    bool canRedo() const noexcept { return canEdit() && context->history->canRedo(); }

    void spawnFromLibrary(const LibraryEntry &entry);
    void removeSelected();
    bool isRemovable(const std::shared_ptr<BaseObject> &object) const;

    void setStatusMessage(std::string message) { context->statusMessage = std::move(message); }
    const std::string &getStatusMessage() const noexcept { return context->statusMessage; }

    float getLastFrameTime() const noexcept { return lastFrameTime; }
    Viewport &getViewport() noexcept { return viewport; }
    ModelLibrary &getModelLibrary() noexcept { return library; }
    std::shared_ptr<EditorContext> getContext() const noexcept { return context; }

   private:
    EditorCore(std::shared_ptr<Scene> scene,
               std::shared_ptr<BasePipeline> pipeline,
               std::shared_ptr<ObjectPickingPass> pickingPass,
               std::shared_ptr<Camera> camera);

    std::shared_ptr<BaseObject> createObject(const LibraryEntry &entry) const;
    glm::vec3 computeSpawnPosition() const;
    void handleShortcuts(const InputState &input);

    std::shared_ptr<EditorContext> context;
    std::unique_ptr<IEditorModeState> modeState;
    Viewport viewport;
    EditorCameraController cameraController;
    ModelLibrary library;
    float lastFrameTime = 0;

    static constexpr float spawnDistance = 5.0f;
    static constexpr float maxGroundDistance = 100.0f;
};
