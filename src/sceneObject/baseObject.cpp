#include "sceneObject/baseObject.h"
#include <atomic>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>

bool isZeroDouble(float val) {
    const float eps = 1e-5;
    return std::fabs(val) < eps;
}

BaseObject::BaseObject() noexcept : id(generateId()) {}

BaseObject::BaseObject(const BaseObject &other)
    : std::enable_shared_from_this<BaseObject>(other),
      id(generateId()),
      name(other.name),
      visible(other.visible),
      position(other.position),
      rotation(other.rotation),
      scale(other.scale) {}

BaseObject &BaseObject::operator=(const BaseObject &other) {
    if (this != &other) {
        name = other.name;
        visible = other.visible;
        position = other.position;
        rotation = other.rotation;
        scale = other.scale;
    }
    return *this;
}

uint32_t BaseObject::generateId() noexcept {
    static std::atomic<uint32_t> nextId{1};
    return nextId++;
}

uint32_t BaseObject::getRootId() const {
    uint32_t rootId = id;
    for (auto ancestor = parent.lock(); ancestor; ancestor = ancestor->parent.lock()) {
        rootId = ancestor->id;
    }
    return rootId;
}

glm::mat4 BaseObject::getModelMatrix() const {
    glm::mat4 mat(1);
    mat = glm::translate(mat, glm::vec3(position));
    if (!isZeroDouble(rotation.z)) {
        mat = glm::rotate(mat, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    }
    if (!isZeroDouble(rotation.y)) {
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
