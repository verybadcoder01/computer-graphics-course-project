#pragma once

#include "renderPipeline/renderPasses/renderPass.h"
#include "renderPipeline/renderPasses/renderPipelineContext.h"
#include "scene/scene.h"

class BasePipeline {
public:
    virtual ~BasePipeline() = default;

    virtual void addPass(std::shared_ptr<IRenderPass> pass) = 0;
    virtual void clearPasses() noexcept = 0;
    virtual void execute(Scene &scene, int width, int height, GLuint targetFBO) = 0;

    virtual RenderPipelineContext& getContext() noexcept = 0;
};
