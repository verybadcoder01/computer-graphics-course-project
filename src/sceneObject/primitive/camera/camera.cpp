#include "camera.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
#include <memory>
#include "visitor/objectVisitor.h"

bool ViewPyramid::intersects(const AABoundBox &box, const glm::mat4 &modelMatrix) const {
    glm::vec3 localCenter = (box.max + box.min) * 0.5f;
    glm::vec3 localExtents = (box.max - box.min) * 0.5f;
    glm::vec3 worldCenter = glm::vec3(modelMatrix * glm::vec4(localCenter, 1.0));

    glm::vec3 right = glm::vec3(modelMatrix[0]);
    glm::vec3 up = glm::vec3(modelMatrix[1]);
    glm::vec3 forward = glm::vec3(modelMatrix[2]);
    for (const auto &plane : planes) {
        float radius = localExtents.x * std::abs(glm::dot(plane.normal, right)) +
                       localExtents.y * std::abs(glm::dot(plane.normal, up)) +
                       localExtents.z * std::abs(glm::dot(plane.normal, forward));
        float signedDistance = plane.getSignedDistanceToPlane(worldCenter);

        if (signedDistance < -radius) {
            return false;
        }
    }
    return true;
}

Camera::Camera(const glm::vec3 &startPosition, float fov, float aspectRatio)
    : fov(fov), aspectRatio(aspectRatio) {
    this->position = glm::vec4(startPosition, 1.0f);
    updateCameraVectors();
}

void Camera::accept(ObjectVisitor &visitor) {
    visitor.visit(*this);
}

glm::mat4 Camera::getViewMatrix() const {
    const glm::vec3 posVec{position.x, position.y, position.z};
    return glm::lookAt(posVec, posVec + front, up);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

std::unique_ptr<ViewPyramid> Camera::getViewPyramid() const {
    std::unique_ptr<ViewPyramid> result = std::make_unique<ViewPyramid>();
    glm::mat4 vp = getProjectionMatrix() * getViewMatrix();

    std::array<glm::vec4, 4> rows;
    for (size_t i = 0; i < 4; ++i) {
        rows[i] = glm::vec4(vp[0][i], vp[1][i], vp[2][i], vp[3][i]);
    }

    for (const auto side : {ViewPyramid::Left, ViewPyramid::Right, ViewPyramid::Bottom,
                            ViewPyramid::Top, ViewPyramid::Near, ViewPyramid::Far}) {
        if (side % 2 == 0) {
            result->setPlane(Plane(rows[3] + rows[side / 2]), side);
        } else {
            result->setPlane(Plane(rows[3] - rows[side / 2]), side);
        }
    }

    glm::mat4 invVP = glm::inverse(vp);
    size_t index = 0;
    for (int x = 0; x < 2; ++x) {
        for (int y = 0; y < 2; ++y) {
            for (int z = 0; z < 2; ++z) {
                glm::vec4 vertexNDC = glm::vec4(x ? 1 : -1, y ? 1 : -1, z ? 1 : -1, 1);
                glm::vec4 worldVertex = invVP * vertexNDC;
                result->setVertex(glm::vec3(worldVertex) / worldVertex.w, index);
                index++;
            }
        }
    }
    return result;
}

void Camera::updateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;
    yaw += xoffset;
    pitch += yoffset;

    if (constrainPitch) {
        pitch = std::min(pitch, 89.0f);
        pitch = std::max(pitch, -89.0f);
    }

    updateCameraVectors();
}

void Camera::processKeyboard(MoveDirection direction, float deltaTime, float speed) {
    float velocity = speed * deltaTime;
    if (direction == MoveDirection::FORWARD) {
        position += glm::vec4(front * velocity, 1.0);
    } else if (direction == MoveDirection::BACKWARD) {
        position -= glm::vec4(front * velocity, 1.0);
    } else if (direction == MoveDirection::LEFT) {
        position -= glm::vec4(right * velocity, 1.0);
    } else if (direction == MoveDirection::RIGHT) {
        position += glm::vec4(right * velocity, 1.0);
    }
}