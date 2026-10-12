#pragma once

#include <memory>
#include "renderPipeline/renderPasses/renderPass.h"
#include "renderPipeline/renderPasses/renderPipelineContext.h"
#include "sceneObject/primitive/model/texture/texture.h"
#include "shaders/shader.h"

class OpaqueRenderPass : public IRenderPass {
   public:
    OpaqueRenderPass(std::shared_ptr<Shader> defaultShader, std::shared_ptr<Shader> particleShader);

    void execute(Scene &scene, RenderPipelineContext &ctx) override;

    void setClearColor(const glm::vec4 &color) noexcept { clearColor = color; }

   protected:
    std::shared_ptr<Shader> defaultShader;
    std::shared_ptr<Shader> particleShader;
    // bound for untextured models and in place of a missing shadow map (white = depth 1, no shadow)
    Texture fallbackTexture;
    glm::vec4 clearColor{0.1f, 0.1f, 0.12f, 1.0f};
};
