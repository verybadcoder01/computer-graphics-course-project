#include "opaqueRenderPass.h"
#include "renderPipeline/renderContext.h"
#include "renderPipeline/renderPasses/renderPipelineContext.h"
#include "visitor/renderVisitor/renderVisitor.h"

OpaqueRenderPass::OpaqueRenderPass(std::shared_ptr<Shader> defaultShader,
                                   std::shared_ptr<Shader> particleShader)
    : defaultShader(std::move(defaultShader)),
      particleShader(std::move(particleShader)),
      fallbackTexture(Texture::createSolidColor({255, 255, 255, 255})) {}

void OpaqueRenderPass::execute(Scene &scene, RenderPipelineContext &ctx) {
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    if (!ctx.camera) {
        return;
    }

    RenderContext renderCtx{.viewMatrix = ctx.camera->getViewMatrix(),
                            .projMatrix = ctx.camera->getProjectionMatrix(),
                            .cameraPos = ctx.camera->getPosition(),
                            .mainLight = ctx.light,
                            .shadowMapTextureId = ctx.shadowMapTextureId};
    OpenGLRenderVisitor renderVisitor(renderCtx, *ctx.gpuCache, *defaultShader, *particleShader,
                                      fallbackTexture);
    scene.accept(renderVisitor);
}
