#include "opaqueRenderPass.h"
#include "renderPipeline/renderContext.h"
#include "renderPipeline/renderPasses/renderPipelineContext.h"
#include "visitor/renderVisitor/renderVisitor.h"

OpaqueRenderPass::OpaqueRenderPass(std::shared_ptr<Shader> defaultShader, std::shared_ptr<Shader> particleShader)
    : defaultShader(std::move(defaultShader)), particleShader(std::move(particleShader)) {}

void OpaqueRenderPass::execute(Scene &scene, RenderPipelineContext &ctx) {
    if (!ctx.camera) {
        return;
    }

    scene.clear();
    RenderContext renderCtx{
        .viewMatrix = ctx.camera->getViewMatrix(),
        .projMatrix = ctx.camera->getProjectionMatrix(),
        .cameraPos = ctx.camera->getPosition(),
        .mainLight = ctx.light, 
        .shadowMapTextureId = ctx.shadowMapTextureId
    };
    OpenGLRenderVisitor renderVisitor(renderCtx, *ctx.gpuCache, *defaultShader, *particleShader);
    scene.accept(renderVisitor);
}
