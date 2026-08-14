#pragma once

#include "renderPipeline/renderPasses/renderPass.h"
#include "renderPipeline/renderPasses/renderPipelineContext.h"
#include "shaders/shader.h"
#include <memory>

class OpaqueRenderPass: public IRenderPass {
public:
    OpaqueRenderPass(std::shared_ptr<Shader> defaultShader, std::shared_ptr<Shader> particleShader);

    void execute(Scene &scene, RenderPipelineContext &ctx) override;

protected:
    std::shared_ptr<Shader> defaultShader;
    std::shared_ptr<Shader> particleShader;
};