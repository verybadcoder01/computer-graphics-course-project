#pragma once

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/geometric.hpp>
#include "sceneObject/primitive/camera/camera.h"
#include "sceneObject/primitive/primitive.h"
#include "visitor/objectVisitor.h"

enum class LightType { Directional = 0, Point, Spot };

struct AttenuationParams {
    float constant = 1;
    float linear = 0.09;
    float quadratic = 0.032;
};

class LightSource : public Primitive {
   public:
    LightSource(const AttenuationParams &atParams, LightType type = LightType::Point)
        : atParams(atParams), type(type) {}
    virtual ~LightSource() override = default;

    void accept(ObjectVisitor &visitor) override { visitor.visit(*this); }

    void updateLightSpaceMatrix(const ViewPyramid &cameraView);
    const glm::mat4 &getLightSpaceMatrix() const { return lightSpaceMatrix; }

    void setDirection(const glm::vec3 &dir) { direction = glm::normalize(dir); }
    const glm::vec3 &getDirection() const { return direction; }

    void setColor(const glm::vec3 &col) { color = col; }
    glm::vec3 getColor() const { return color * intensity; }

    const AttenuationParams &getAttenuationParams() const { return atParams; }

    bool isVisible() const noexcept override { return false; }
    bool createsShadows() const noexcept { return castsShadows; }
    LightType getType() const noexcept { return type; }

    void update(float dt) override {};

   protected:
    LightType type = LightType::Point;
    glm::vec3 color{1.0f};
    glm::vec3 direction{0, -1, 0};
    float intensity = 1;
    bool castsShadows = true;

    AttenuationParams atParams;
    glm::mat4 lightSpaceMatrix{1.0};
};