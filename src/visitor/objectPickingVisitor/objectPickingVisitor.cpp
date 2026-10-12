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
    if (!model.isVisible()) {
        return;
    }
    const auto &handles = cache.get().getOrUpload<Model>(model, MeshUploader::uploadModelToGPU);

    const Shader &shader = pickingShader.get();
    shader.use();
    shader.setMat4("u_Projection", projMatrix);
    shader.setMat4("u_View", viewMatrix);
    shader.setMat4("u_Model", model.getWorldMatrix());
    // parts of a composite or an emitter's nozzle select the top-level scene object
    shader.setUInt("u_ObjectID", model.getRootId());

    glBindVertexArray(handles.getVAO());
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(handles.getElementCount()), GL_UNSIGNED_INT,
                   nullptr);
    glBindVertexArray(0);
}
