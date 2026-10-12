#include "objectPickingPass.h"
#include "renderPipeline/renderPasses/renderPipelineContext.h"
#include "shaders/shader.h"
#include "visitor/objectPickingVisitor/objectPickingVisitor.h"

ObjectPickingPass::ObjectPickingPass(std::shared_ptr<Shader> pickingShader, int width, int height)
    : pickingShader(std::move(pickingShader)),
      pickingFBO(
          std::make_unique<ViewportFramebuffer>(width, height, FramebufferColorFormat::R32UI)) {}

void ObjectPickingPass::execute(Scene &scene, RenderPipelineContext &ctx) {
    if (!ctx.camera) {
        return;
    }
    if (ctx.screenWidth > 0 && ctx.screenHeight > 0 &&
        (pickingFBO->getWidth() != ctx.screenWidth ||
         pickingFBO->getHeight() != ctx.screenHeight)) {
        pickingFBO->resize(ctx.screenWidth, ctx.screenHeight);
    }

    pickingFBO->bind();
    glViewport(0, 0, pickingFBO->getWidth(), pickingFBO->getHeight());

    glDisable(GL_BLEND);
    glDisable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    pickingFBO->fillUint(0);

    ObjectPickingVisitor visitor(*ctx.gpuCache, *pickingShader, *ctx.camera);
    scene.accept(visitor);

    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);
    pickingFBO->unbind();
}

uint32_t ObjectPickingPass::readObjectAt(int x, int y) {
    if (x < 0 || y < 0 || x >= pickingFBO->getWidth() || y >= pickingFBO->getHeight()) {
        return 0;
    }
    pickingFBO->bind();
    uint32_t id = 0;
    glReadPixels(x, pickingFBO->getHeight() - 1 - y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &id);
    pickingFBO->unbind();
    return id;
}
