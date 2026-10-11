#pragma once

#include <functional>
#include "renderPipeline/gpuCache/gpuCacheManager.h"
#include "sceneObject/primitive/camera/camera.h"
#include "sceneObject/primitive/lightSource/lightSource.h"
#include "sceneObject/primitive/model/model.h"
#include "sceneObject/primitive/particleEmitter/particleEmitter.h"
#include "shaders/shader.h"
#include "visitor/objectVisitor.h"

class ObjectPickingVisitor : public ObjectVisitor {
   public:
    ObjectPickingVisitor(GPUCacheManager &cache, const Shader &pickingShader, const Camera &camera);

    void visit(Model &model) override;
    void visit(ParticleEmitter &emitter) override {};
    void visit(Camera &camera) override {};
    void visit(LightSource &light) override {};

   protected:
    std::reference_wrapper<GPUCacheManager> cache;
    std::reference_wrapper<const Shader> pickingShader;

    glm::mat4 viewMatrix{1}, projMatrix{1};
};