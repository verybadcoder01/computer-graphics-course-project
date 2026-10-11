#include "renderPipeline.h"
#include <memory>
#include "renderPipeline/gpuCache/gpuCacheManager.h"
#include "renderPipeline/renderPasses/renderPass.h"
#include "scene/scene.h"

RenderPipeline::RenderPipeline(std::shared_ptr<GPUCacheManager> cache) {
    context.gpuCache = std::move(cache);
}

void RenderPipeline::addPass(std::shared_ptr<IRenderPass> pass) {
    if (pass) {
        passes.push_back(std::move(pass));
    }
}

void RenderPipeline::clearPasses() noexcept {
    passes.clear();
}

void RenderPipeline::execute(Scene &scene, int width, int height, GLuint targetFBO = 0) {
    context.screenWidth = width;
    context.screenHeight = height;
    context.targetFrameBuffer = targetFBO;

    glBindFramebuffer(GL_FRAMEBUFFER, context.targetFrameBuffer);
    glViewport(0, 0, width, height);

    for (auto &pass : passes) {
        if (pass->isEnabled()) {
            pass->execute(scene, context);
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if (context.gpuCache) {
        context.gpuCache->cleanupExpired();
    }
}
