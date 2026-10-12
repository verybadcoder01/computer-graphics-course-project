#include "composite.h"
#include <algorithm>
#include <format>
#include <source_location>
#include "exceptions/scene/sceneExceptions.h"

void ObjectsComposite::accept(ObjectVisitor &visitor) {
    for (const auto &child : children) {
        child->accept(visitor);
    }
}

void ObjectsComposite::update(const SimulationContext &ctx) {
    for (const auto &child : children) {
        child->update(ctx);
    }
}

void ObjectsComposite::add(std::shared_ptr<BaseObject> child) {
    if (!child) {
        throw InvalidSceneObjectException("Cannot add a null child to a composite",
                                          std::source_location::current());
    }
    child->setParent(weak_from_this());
    children.push_back(std::move(child));
}

void ObjectsComposite::remove(uint32_t childId) {
    auto it = std::find_if(children.begin(), children.end(),
                           [childId](const auto &child) { return child->getId() == childId; });
    if (it == children.end()) {
        throw ObjectKeyNotFoundException(std::format("Child {} not found", childId),
                                         std::source_location::current());
    }
    (*it)->setParent({});
    children.erase(it);
}
