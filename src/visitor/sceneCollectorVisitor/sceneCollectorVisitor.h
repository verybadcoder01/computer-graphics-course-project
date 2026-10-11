#pragma once

#include <memory>
#include "sceneObject/primitive/camera/camera.h"
#include "sceneObject/primitive/lightSource/lightSource.h"
#include "visitor/objectVisitor.h"

class SceneCollectorVisitor : public ObjectVisitor {
   public:
    SceneCollectorVisitor() = default;
    virtual ~SceneCollectorVisitor() = default;

    void visit(Camera &camera) override;
    void visit(LightSource &light) override;
    void visit(Model &) override {}
    void visit(ParticleEmitter &) override {}

    std::shared_ptr<Camera> getCamera() const noexcept { return activeCamera; }
    std::shared_ptr<LightSource> getLight() const noexcept { return activeLight; }

   protected:
    std::shared_ptr<Camera> activeCamera = nullptr;
    std::shared_ptr<LightSource> activeLight = nullptr;
};