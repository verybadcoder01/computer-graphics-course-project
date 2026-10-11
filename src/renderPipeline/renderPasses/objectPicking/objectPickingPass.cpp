#include "objectPickingPass.h"
#include "renderPipeline/renderPasses/renderPipelineContext.h"
#include "shaders/shader.h"
#include "visitor/objectPickingVisitor/objectPickingVisitor.h"

void ObjectPickingPass::execute(Scene &scene, RenderPipelineContext &ctx) {
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
    pickingFBO->bind();
    uint32_t id = 0;
    glReadPixels(x, pickingFBO->getHeight() - y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &id);
    pickingFBO->unbind();
    return id;
}
