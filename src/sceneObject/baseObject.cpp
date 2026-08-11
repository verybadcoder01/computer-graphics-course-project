#include "sceneObject/baseObject.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>

bool isZeroDouble(float val) {
    const float eps = 1e-5;
    return std::fabs(val) < eps;
}

glm::mat4 BaseObject::getModelMatrix() const {
    glm::mat4 mat(1);
    mat = glm::translate(mat, glm::vec3(position));
    if (!isZeroDouble(rotation.z)){
        mat = glm::rotate(mat, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    }
    if (!isZeroDouble(rotation.y)){
        mat = glm::rotate(mat, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    }
    if (!isZeroDouble(rotation.x)) {
        mat = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    }
    mat = glm::scale(mat, scale);
    return mat;
}

glm::mat4 BaseObject::getWorldMatrix() const {
    glm::mat4 localMatrix = getModelMatrix();
    if (auto parentPtr = parent.lock()) {
        return parentPtr->getWorldMatrix() * localMatrix;
    }
    return localMatrix;
}
