#pragma once

#include <functional>
#include "renderPipeline/gpuCache/gpuCacheManager.h"
#include "sceneObject/primitive/camera/camera.h"
#include "sceneObject/primitive/lightSource/lightSource.h"
#include "sceneObject/primitive/model/model.h"
#include "sceneObject/primitive/particleEmitter/particleEmitter.h"
#include "shaders/shader.h"
#include "visitor/objectVisitor.h"

class ShadowDepthVisitor : public ObjectVisitor {
   public:
    ShadowDepthVisitor(GPUCacheManager &cache,
                       Shader &depthShader,
                       const glm::mat4 &lightSpaceMatrix);

    void visit(Model &model) override;
    void visit(ParticleEmitter &emitter) override {};
    void visit(Camera &camera) override {};
    void visit(LightSource &light) override {};

   protected:
    std::reference_wrapper<GPUCacheManager> cache;
    std::reference_wrapper<Shader> shader;
    glm::mat4 lightSpaceMatrix;

    GPUBufferHandles uploadToGPU(const Model &model);
};