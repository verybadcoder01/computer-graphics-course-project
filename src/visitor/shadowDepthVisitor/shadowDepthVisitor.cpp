#include "shadowDepthVisitor.h"
#include "renderPipeline/gpuCache/gpuCacheManager.h"
#include "renderPipeline/meshUploader/meshUploader.h"

ShadowDepthVisitor::ShadowDepthVisitor(GPUCacheManager &cache,
                                       Shader &depthShader,
                                       const glm::mat4 &lightSpaceMatrix)
    : cache(cache), shader(depthShader), lightSpaceMatrix(lightSpaceMatrix) {}

void ShadowDepthVisitor::visit(Model &model) {
    const auto &handles = cache.get().getOrUpload<Model>(model, MeshUploader::uploadModelToGPU);

    shader.get().use();
    shader.get().setMat4("u_LightSpaceMatrix", lightSpaceMatrix);
    shader.get().setMat4("u_Model", model.getModelMatrix());

    glBindVertexArray(handles.getVAO());
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(handles.getElementCount()), GL_UNSIGNED_INT,
                   nullptr);
    glBindVertexArray(0);
}
