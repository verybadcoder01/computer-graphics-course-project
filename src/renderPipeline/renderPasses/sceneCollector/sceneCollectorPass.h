#pragma once

#include "renderPipeline/renderPasses/renderPass.h"
#include "renderPipeline/renderPasses/renderPipelineContext.h"
#include "visitor/sceneCollectorVisitor/sceneCollectorVisitor.h"

class SceneCollectorPass : public IRenderPass {
   public:
    void execute(Scene &scene, RenderPipelineContext &ctx) override {
        SceneCollectorVisitor collector;
        scene.accept(collector);

        ctx.camera = collector.getCamera();
        ctx.light = collector.getLight();
    }
};
