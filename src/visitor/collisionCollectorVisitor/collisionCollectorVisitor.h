#pragma once

#include "sceneObject/collider/baseCollider.h"
#include "sceneObject/primitive/camera/camera.h"
#include "sceneObject/primitive/lightSource/lightSource.h"
#include "visitor/objectVisitor.h"

class CollisionCollectorVisitor : public ObjectVisitor {
   public:
    CollisionCollectorVisitor() = default;
    virtual ~CollisionCollectorVisitor() = default;

    void visit(Camera &camera) override {}
    void visit(LightSource &light) override {}
    void visit(Model &) override;
    void visit(ParticleEmitter &) override {}

    const CollisionEnvironment &getColliders() const noexcept { return colliders; }

   protected:
    CollisionEnvironment colliders;
};
