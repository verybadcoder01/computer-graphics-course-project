#pragma once

#include <memory>
#include <vector>
#include "renderPipeline/basePipeline.h"
#include "renderPipeline/gpuCache/gpuCacheManager.h"
#include "renderPipeline/renderPasses/renderPass.h"
#include "renderPipeline/renderPasses/renderPipelineContext.h"
#include "scene/scene.h"

class RenderPipeline : public BasePipeline {
   public:
    explicit RenderPipeline(std::shared_ptr<GPUCacheManager> cache);

    void addPass(std::shared_ptr<IRenderPass> pass) override;
    void clearPasses() noexcept override;
    void execute(Scene &scene, int width, int height, GLuint targetFBO) override;
    RenderPipelineContext &getContext() noexcept override { return context; }

   protected:
    RenderPipelineContext context;
    std::vector<std::shared_ptr<IRenderPass>> passes;
};
