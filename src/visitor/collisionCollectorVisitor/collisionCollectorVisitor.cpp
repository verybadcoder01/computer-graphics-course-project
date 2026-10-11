#include "collisionCollectorVisitor.h"
#include <memory>
#include "sceneObject/collider/baseCollider.h"
#include "sceneObject/primitive/model/model.h"

void CollisionCollectorVisitor::visit(Model &model) {
    std::shared_ptr<Collider> curCollider = model.getCollider();
    
    colliders.push_back(curCollider);
}