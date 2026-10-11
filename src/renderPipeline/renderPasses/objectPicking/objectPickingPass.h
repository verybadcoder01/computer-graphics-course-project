#pragma once

#include <memory>
#include "renderPipeline/framebuffer/viewportFramebuffer.h"
#include "renderPipeline/renderPasses/renderPass.h"
#include "renderPipeline/renderPasses/renderPipelineContext.h"
#include "shaders/shader.h"

class ObjectPickingPass : public IRenderPass {
   public:
    ObjectPickingPass(std::shared_ptr<Shader> pickingShader, int width, int height);

    void execute(Scene &scene, RenderPipelineContext &ctx) override;
    uint32_t readObjectAt(int x, int y);

   protected:
    std::shared_ptr<Shader> pickingShader;
    std::unique_ptr<ViewportFramebuffer> pickingFBO;
};