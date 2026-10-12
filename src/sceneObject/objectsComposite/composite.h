#pragma once

#include <vector>
#include "sceneObject/baseObject.h"

class ObjectsComposite : public BaseObject {
   public:
    virtual ~ObjectsComposite() = default;

    void accept(ObjectVisitor &visitor) override;
    void update(const SimulationContext &ctx) override;

    // must be called on a composite owned by a shared_ptr (the child's parent is weak_from_this())
    void add(std::shared_ptr<BaseObject> child);
    void remove(uint32_t childId);

    const std::vector<std::shared_ptr<BaseObject>> &getChildren() const noexcept {
        return children;
    }

   protected:
    std::vector<std::shared_ptr<BaseObject>> children;
};
