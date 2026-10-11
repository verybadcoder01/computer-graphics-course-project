#pragma once
#include "editor/editorContext.h"
#include "renderPipeline/renderPasses/objectPicking/objectPickingPass.h"
#include <memory>

class EditorCore : public std::enable_shared_from_this<EditorCore> {
public:
    static std::shared_ptr<EditorCore> create(
        std::shared_ptr<Scene> scene,
        std::shared_ptr<BasePipeline> pipeline,
        std::shared_ptr<ObjectPickingPass> pickingPass
    );

    void update(float dt);
    void handleViewportClick(int mouseX, int mouseY);

    void executeCommand(std::unique_ptr<ICommand> cmd);
    void undo();
    void redo();

    void setSimulationState(bool running) noexcept;
    bool isSimulating() const noexcept;
    std::shared_ptr<EditorContext> getContext() const noexcept { return context; }

private:
    EditorCore(
        std::shared_ptr<Scene> scene,
        std::shared_ptr<BasePipeline> pipeline,
        std::shared_ptr<ObjectPickingPass> pickingPass
    );

    std::shared_ptr<EditorContext> context;
    std::shared_ptr<ObjectPickingPass> pickingPass;
};
