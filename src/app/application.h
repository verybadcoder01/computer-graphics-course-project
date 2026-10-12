#pragma once

#include <memory>
#include "adapters/gui/guiAdapter.h"
#include "app/window/window.h"
#include "editor/core/editorCore.h"
#include "editor/ui/editorUI.h"
#include "renderPipeline/gpuCache/gpuCacheManager.h"
#include "renderPipeline/pipeline/renderPipeline.h"
#include "renderPipeline/renderPasses/objectPicking/objectPickingPass.h"
#include "scene/scene.h"
#include "shadow/shadowManager/shadowManager.h"

class Application {
   public:
    Application();
    ~Application();

    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;

    void run();

   private:
    // declaration order matters: members are destroyed in reverse, the window (GL context) last
    std::unique_ptr<Window> window;
    std::unique_ptr<IGuiAdapter> gui;
    std::shared_ptr<GPUCacheManager> gpuCache;
    std::shared_ptr<ShadowManager> shadowManager;
    std::shared_ptr<ObjectPickingPass> pickingPass;
    std::shared_ptr<RenderPipeline> pipeline;
    std::shared_ptr<Scene> scene;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<EditorCore> editor;
    std::unique_ptr<EditorUI> ui;

    static constexpr int initialWidth = 1600;
    static constexpr int initialHeight = 900;
    // caps dt after a stall (window drag, breakpoint) so the simulation doesn't jump
    static constexpr float maxFrameTime = 0.05f;

    void buildPipeline();
    void buildDefaultScene();
};
