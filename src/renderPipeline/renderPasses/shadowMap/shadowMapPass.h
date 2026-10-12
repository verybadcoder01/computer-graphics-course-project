#pragma once

#include <memory>
#include "renderPipeline/renderPasses/renderPass.h"
#include "renderPipeline/renderPasses/renderPipelineContext.h"
#include "shadow/shadowManager/shadowManager.h"

class ShadowMapPass : public IRenderPass {
   public:
    explicit ShadowMapPass(std::shared_ptr<ShadowManager> shadowMgr);

    void execute(Scene &scene, RenderPipelineContext &rCtx) override;

   protected:
    std::shared_ptr<ShadowManager> shadowMgr;
};
