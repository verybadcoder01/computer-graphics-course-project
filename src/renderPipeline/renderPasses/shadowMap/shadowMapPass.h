#pragma once

#include "renderPipeline/renderPasses/renderPipelineContext.h"
#include "shadow/shadowManager/shadowManager.h"
#include "renderPipeline/renderPasses/renderPass.h"
#include <memory>

class ShadowMapPass: public IRenderPass {
public:
    ShadowMapPass(std::shared_ptr<ShadowManager> shadowMgr, std::shared_ptr<Shader> shadowShader);

    void execute(Scene &scene, RenderPipelineContext &rCtx) override;

protected:
    std::shared_ptr<ShadowManager> shadowMgr;
    std::shared_ptr<Shader> shadowShader;
};
