#pragma once

#include "sceneObject/primitive/primitive.h"
#include "visitor/objectVisitor.h"
#include <array>
#include <glm/geometric.hpp>
#include <memory>

enum MoveDirection {
    FORWARD = 0,
    LEFT,
    RIGHT,
    BACKWARD
};

struct Plane {
    glm::vec3 normal{0, 1, 0};
    float distance = 0;

    Plane() = default;
    Plane(const glm::vec3& p1, const glm::vec3 &norm): normal(glm::normalize(norm)), distance(-glm::dot(normal, p1)) {}
    Plane(const glm::vec4& p) {
        float length = glm::length(glm::vec3(p));
        normal = glm::vec3(p) / length;
        distance = p.w / length;
    }

    float getSignedDistanceToPlane(const glm::vec3& point) const {
        return glm::dot(normal, point) + distance;
    }
};

struct AABoundBox {
    glm::vec3 min{-1};
    glm::vec3 max{1};
};

class ViewPyramid {
public:
    // note: do not reorder
    enum Side {
        Left = 0,
        Right,
        Bottom,
        Top,
        Near,
        Far
    };

    const std::array<glm::vec3, 8>& getVertexes() const { return vertexes; }

    void setPlane(const Plane &p, Side index) { planes[index] = p; }
    void setVertex(const glm::vec3 &v, size_t index) { vertexes[index] = v; }

    bool intersects(const AABoundBox &box, const glm::mat4 &modelMatrix) const;

private:
    std::array<Plane, 6> planes;
    std::array<glm::vec3, 8> vertexes;
};

class Camera : public Primitive {
public:
    Camera() = default;
    Camera(const glm::vec3 &startPosition, float fov = 45, float aspectRatio = 16.0f / 9.0f);
    virtual ~Camera() override = default;

    bool isVisible() const noexcept override { return false; }
    void accept(ObjectVisitor &visitor) override;

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    std::unique_ptr<ViewPyramid> getViewPyramid() const;

    void setAspectRatio(float ratio) noexcept { aspectRatio = ratio; }
    void setFov(float newFov) noexcept { fov = newFov; }
    void setPlanes(float nearP, float farP) noexcept { nearPlane = nearP; farPlane = farP; }

    void updateCameraVectors();
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void processKeyboard(MoveDirection direction, float deltaTime, float speed);

protected:
    glm::vec3 front{0, 0, -1}, up{0, 1, 0}, right{1, 0, 0};\
    const glm::vec3 worldUp{0, 1, 0};
    float fov = 45, aspectRatio = 16.0f / 9.0f, nearPlane = 0.1, farPlane = 100.0;

    float yaw = -90, pitch = 0;
    float mouseSensitivity = 0.1;
};