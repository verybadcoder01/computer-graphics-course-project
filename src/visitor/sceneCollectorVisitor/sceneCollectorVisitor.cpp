#include "sceneCollectorVisitor.h"

void SceneCollectorVisitor::visit(Camera& camera) {
    activeCamera = std::static_pointer_cast<Camera>(camera.shared_from_this());
}

void SceneCollectorVisitor::visit(LightSource& light) {
    if (!activeLight || light.createsShadows()) {
        activeLight = std::static_pointer_cast<LightSource>(light.shared_from_this());
    }
}
