#pragma once

#include "renderPipeline/gpuCache/gpuCacheManager.h"
#include "sceneObject/primitive/model/model.h"
#include "sceneObject/primitive/particleEmitter/particleEmitter.h"

class MeshUploader {
   public:
    static GPUBufferHandles uploadModelToGPU(const Model &model);
    static GPUBufferHandles uploadParticleEmitterToGPU(const ParticleEmitter &emitter);
};
