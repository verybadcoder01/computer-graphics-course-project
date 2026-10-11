#include "hemisphereCollider.h"
#include "sceneObject/primitive/particle/particle.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/projection.hpp"

bool ThickHemisphereCollider::checkCollision(Particle &p) const {
    glm::vec3 pos = p.getPosition();
    glm::vec3 dir = pos - center;
    float r = glm::length(dir);
    if (r < 0.0001f) {
        return true;
    }
    float h = glm::dot(dir, normal);

    if (h >= -p.getRadius()) {
        glm::vec3 unitDir = dir / r;
        if (r > innerRadius - p.getRadius() && r < innerRadius && h > 0.0f) {
            p.setPosition(center + unitDir * (innerRadius - p.getRadius()));
            glm::vec3 normalVel = glm::proj(p.getVelocity(), -unitDir);
            p.setVelocity((p.getVelocity() - normalVel) - normalVel * bounceCoeff);
            return true;
        }

        if (r >= innerRadius && r < outerRadius + p.getRadius() && h > 0.0f) {
            float distToInner = r - innerRadius;
            float distToOuter = outerRadius - r;
            if (distToOuter < distToInner) {
                p.setPosition(center + unitDir * (outerRadius + p.getRadius()));
                glm::vec3 n = unitDir;
                p.setVelocity(glm::reflect(p.getVelocity(), n) * bounceCoeff);
            } else {
                p.setPosition(center + unitDir * (innerRadius - p.getRadius()));
                glm::vec3 n = -unitDir;
                p.setVelocity(glm::reflect(p.getVelocity(), n) * bounceCoeff);
            }
            return true;
        }

        float projDist = glm::length(dir - normal * h);
        if (h >= -p.getRadius() && h <= p.getRadius() && projDist >= innerRadius && projDist <= outerRadius) {
            p.setPosition(pos + normal * (p.getRadius() - h));
            p.setVelocity(glm::reflect(p.getVelocity(), normal) * bounceCoeff);
            return true;
        }
    }
    return false;
}
