#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include "glm/glm.hpp"
#include "renderPipeline/basePipeline.h"
#include "renderPipeline/framebuffer/viewportFramebuffer.h"
#include "renderPipeline/renderPasses/objectPicking/objectPickingPass.h"
#include "sceneObject/primitive/camera/camera.h"

class Viewport {
   public:
    Viewport(std::shared_ptr<BasePipeline> pipeline,
             std::shared_ptr<ObjectPickingPass> pickingPass,
             std::shared_ptr<Camera> camera);

    void resize(int width, int height);
    // viewport-local pixel coordinates, origin at the top left
    void requestPick(int x, int y) noexcept;
    std::optional<uint32_t> render(Scene &scene);

    GLuint getColorTexture() const noexcept { return framebuffer.getColorTexture(); }
    glm::ivec2 getSize() const noexcept {
        return {framebuffer.getWidth(), framebuffer.getHeight()};
    }

   private:
    std::shared_ptr<BasePipeline> pipeline;
    std::shared_ptr<ObjectPickingPass> pickingPass;
    std::shared_ptr<Camera> camera;
    ViewportFramebuffer framebuffer;
    std::optional<glm::ivec2> pendingPick;
};
