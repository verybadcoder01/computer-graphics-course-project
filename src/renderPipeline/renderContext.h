#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include "sceneObject/primitive/lightSource/lightSource.h"

class LightSource;

struct RenderContext {
    glm::mat4 viewMatrix{1.0f};
    glm::mat4 projMatrix{1.0f};
    glm::vec3 cameraPos{0.0f};

    std::shared_ptr<LightSource> mainLight = nullptr;
    GLuint shadowMapTextureId = 0;
};