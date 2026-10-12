#include "shadowMapPass.h"
#include "renderPipeline/renderPasses/renderPipelineContext.h"
#include "scene/scene.h"
#include "visitor/shadowDepthVisitor/shadowDepthVisitor.h"

ShadowMapPass::ShadowMapPass(std::shared_ptr<ShadowManager> shadowMgr)
    : shadowMgr(std::move(shadowMgr)) {}

void ShadowMapPass::execute(Scene &scene, RenderPipelineContext &rCtx) {
    if (!rCtx.light || !rCtx.light->createsShadows() || !rCtx.camera || !shadowMgr) {
        return;
    }

    rCtx.light->updateLightSpaceMatrix(*rCtx.camera->getViewPyramid());
    shadowMgr->beginShadowPass();

    ShadowDepthVisitor depthVisitor(*rCtx.gpuCache, *shadowMgr->getShadowShader(),
                                    rCtx.light->getLightSpaceMatrix());
    scene.accept(depthVisitor);

    shadowMgr->endShadowPass(rCtx.screenWidth, rCtx.screenHeight);
    rCtx.shadowMapTextureId = shadowMgr->getDepthTexture();
}
