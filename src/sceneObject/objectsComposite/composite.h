#pragma once

#include <vector>
#include "sceneObject/baseObject.h"
class ObjectsComposite : public BaseObject {
   public:
    virtual ~ObjectsComposite() = default;
    void add(std::shared_ptr<BaseObject> child);
    void remove(uint32_t index);

   protected:
    std::vector<std::shared_ptr<BaseObject>> children;
};