#include <glad/glad.h>

#include <algorithm>
#include "adapters/gui/imgui/imguiAdapter.h"
#include "application.h"
#include "renderPipeline/renderPasses/opaqueRender/opaqueRenderPass.h"
#include "renderPipeline/renderPasses/sceneCollector/sceneCollectorPass.h"
#include "renderPipeline/renderPasses/shadowMap/shadowMapPass.h"
#include "sceneObject/primitive/lightSource/lightSource.h"
#include "shaders/shader.h"

Application::Application() {
    window = std::make_unique<Window>(initialWidth, initialHeight, "Fountain Editor");
    gui = std::make_unique<ImGuiAdapter>();
    gui->init(*window);

    buildPipeline();
    buildDefaultScene();

    editor = EditorCore::create(scene, pipeline, pickingPass, camera);
    ui = std::make_unique<EditorUI>();
}

Application::~Application() {
    ui.reset();
    editor.reset();
    // the scene singleton outlives this object; free its GL resources while the context exists
    if (scene) {
        scene->clearObjects();
    }
}

void Application::buildPipeline() {
    gpuCache = std::make_shared<GPUCacheManager>();
    shadowManager = std::make_shared<ShadowManager>();

    auto defaultShader = std::make_shared<Shader>("src/shaders/default/default.vs",
                                                  "src/shaders/default/default.fs");
    auto particleShader = std::make_shared<Shader>("src/shaders/particle/particle.vs",
                                                   "src/shaders/particle/particle.fs");
    auto pickingShader = std::make_shared<Shader>("src/shaders/picking/picking.vs",
                                                  "src/shaders/picking/picking.fs");

    pickingPass = std::make_shared<ObjectPickingPass>(pickingShader, 1, 1);
    pipeline = std::make_shared<RenderPipeline>(gpuCache);
    pipeline->addPass(std::make_shared<SceneCollectorPass>());
    pipeline->addPass(std::make_shared<ShadowMapPass>(shadowManager));
    pipeline->addPass(std::make_shared<OpaqueRenderPass>(defaultShader, particleShader));
    pipeline->addPass(pickingPass);
}

void Application::buildDefaultScene() {
    scene = Scene::getInstance();

    camera = std::make_shared<Camera>(glm::vec3(0.0f, 3.0f, 10.0f));
    camera->setName("Camera");
    scene->addObject(camera);

    auto sun = std::make_shared<LightSource>(AttenuationParams{}, LightType::Directional);
    sun->setName("Sun");
    sun->setPosition({0.0f, 10.0f, 0.0f});
    sun->setDirection({-0.4f, -1.0f, -0.3f});
    scene->addObject(sun);
}

void Application::run() {
    double lastTime = window->getTime();
    while (!window->shouldClose()) {
        window->pollEvents();
        const double now = window->getTime();
        const float dt = std::min(static_cast<float>(now - lastTime), maxFrameTime);
        lastTime = now;

        gui->beginFrame();
        ui->draw(*gui, *editor);
        editor->update(dt, gui->getInput());
        editor->renderViewport();

        const glm::ivec2 framebufferSize = window->getFramebufferSize();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, framebufferSize.x, framebufferSize.y);
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        gui->endFrame();

        window->swapBuffers();
    }
}
