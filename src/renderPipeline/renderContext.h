#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include "sceneObject/primitive/lightSource/lightSource.h"
#include <optional>

class LightSource;

struct RenderContext {
    glm::mat4 viewMatrix{1.0f};
    glm::mat4 projMatrix{1.0f};
    glm::vec3 cameraPos{0.0f};
    
    std::optional<LightSource> mainLight = std::nullopt;
    GLuint shadowMapTextureId = 0;
};