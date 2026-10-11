#include "objectPickingVisitor.h"
#include "renderPipeline/meshUploader/meshUploader.h"
#include "shaders/shader.h"

ObjectPickingVisitor::ObjectPickingVisitor(GPUCacheManager &cache,
                                           const Shader &pickingShader,
                                           const Camera &camera)
    : cache(cache),
      pickingShader(pickingShader),
      viewMatrix(camera.getViewMatrix()),
      projMatrix(camera.getProjectionMatrix()) {}

void ObjectPickingVisitor::visit(Model &model) {
    const auto &handles = cache.get().getOrUpload<Model>(model, MeshUploader::uploadModelToGPU);

    const Shader &shader = pickingShader.get();
    shader.use();
    shader.setMat4("projection", projMatrix);
    shader.setMat4("view", viewMatrix);
    shader.setMat4("model", model.getModelMatrix());
    shader.setUInt("u_ObjectID", model.getId());

    glBindVertexArray(handles.getVAO());
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(handles.getElementCount()), GL_UNSIGNED_INT,
                   nullptr);
    glBindVertexArray(0);
}
