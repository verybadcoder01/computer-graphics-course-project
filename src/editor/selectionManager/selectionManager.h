#pragma once
#include <functional>
#include <memory>
#include "scene/scene.h"
#include "sceneObject/baseObject.h"

class SelectionManager {
   public:
    using OnSelectionChanged = std::function<void(std::shared_ptr<BaseObject>)>;

    void select(std::shared_ptr<BaseObject> object) {
        if (selectedObject.lock() != object) {
            selectedObject = object;
            if (onSelectionChanged) {
                onSelectionChanged(selectedObject.lock());
            }
        }
    }

    void selectByID(const std::shared_ptr<Scene> &scene, uint32_t id) {
        if (!scene || id == 0 || !scene->contains(id)) {
            clearSelection();
            return;
        }
        const auto &obj = scene->getObject(id);
        select(obj);
    }

    void clearSelection() { select(nullptr); }

    std::shared_ptr<BaseObject> getSelected() const noexcept { return selectedObject.lock(); }

    bool hasSelection() const noexcept { return !selectedObject.expired(); }

    void setCallback(OnSelectionChanged callback) { onSelectionChanged = std::move(callback); }

   private:
    std::weak_ptr<BaseObject> selectedObject;
    OnSelectionChanged onSelectionChanged;
};
