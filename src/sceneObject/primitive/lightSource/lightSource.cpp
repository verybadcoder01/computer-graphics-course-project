#include "lightSource.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <limits>

void LightSource::updateLightSpaceMatrix(const ViewPyramid &cameraView) {
    if (type != LightType::Directional) {
        return;
    }

    glm::vec3 lookAtTarget = glm::vec3(position) + direction;
    glm::mat4 lightView = glm::lookAt(glm::vec3(position), lookAtTarget, glm::vec3(0, 1, 0));
    glm::vec3 minAABB(std::numeric_limits<float>::max());
    glm::vec3 maxAABB(std::numeric_limits<float>::lowest());

    for (const auto &vertex : cameraView.getVertexes()) {
        glm::vec4 trVertex = lightView * glm::vec4(vertex, 1.0f);

        minAABB.x = std::min(minAABB.x, trVertex.x);
        maxAABB.x = std::max(maxAABB.x, trVertex.x);

        minAABB.y = std::min(minAABB.y, trVertex.y);
        maxAABB.y = std::max(maxAABB.y, trVertex.y);

        minAABB.z = std::min(minAABB.z, trVertex.z);
        maxAABB.z = std::max(maxAABB.z, trVertex.z);
    }

    float zMultOutsideCamera = 10;
    if (minAABB.z < 0) {
        minAABB.z *= zMultOutsideCamera;
    } else {
        minAABB.z /= zMultOutsideCamera;
    }
    if (maxAABB.z < 0) {
        maxAABB.z /= zMultOutsideCamera;
    } else {
        maxAABB.z *= zMultOutsideCamera;
    }

    float zNear = -maxAABB.z;
    float zFar = -minAABB.z;
    glm::mat4 lightProjection = glm::ortho(minAABB.x, maxAABB.x, minAABB.y, maxAABB.y, zNear, zFar);
    lightSpaceMatrix = lightProjection * lightView;
}