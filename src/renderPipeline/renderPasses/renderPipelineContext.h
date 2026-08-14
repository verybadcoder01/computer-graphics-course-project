#pragma once

#include "sceneObject/primitive/camera/camera.h"
#include "sceneObject/primitive/lightSource/lightSource.h"
#include "renderPipeline/gpuCache/gpuCacheManager.h"
#include <glm/glm.hpp>
#include <memory>
#include <glad/glad.h>

struct RenderPipelineContext {
    int screenWidth = 0;
    int screenHeight = 0;
    std::shared_ptr<Camera> camera = nullptr;
    std::shared_ptr<LightSource> light = nullptr;
    GLuint shadowMapTextureId = 0;
    GLuint targetFrameBuffer = 0;
    std::shared_ptr<GPUCacheManager> gpuCache;
};
