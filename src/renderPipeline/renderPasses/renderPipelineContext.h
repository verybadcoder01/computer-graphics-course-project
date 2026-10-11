#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include "renderPipeline/gpuCache/gpuCacheManager.h"
#include "sceneObject/primitive/camera/camera.h"
#include "sceneObject/primitive/lightSource/lightSource.h"

struct RenderPipelineContext {
    int screenWidth = 0;
    int screenHeight = 0;
    std::shared_ptr<Camera> camera = nullptr;
    std::shared_ptr<LightSource> light = nullptr;
    GLuint shadowMapTextureId = 0;
    GLuint targetFrameBuffer = 0;
    std::shared_ptr<GPUCacheManager> gpuCache;
};
