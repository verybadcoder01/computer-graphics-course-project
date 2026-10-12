#pragma once

#include <memory>
#include "editor/command/commandInterface.h"
#include "sceneObject/primitive/model/model.h"

class SetColliderCommand : public ICommand {
   public:
    // newCollider may be nullptr (removes the collider)
    SetColliderCommand(std::shared_ptr<Model> model, std::shared_ptr<Collider> newCollider)
        : model(std::move(model)), newCollider(std::move(newCollider)) {
        oldCollider = this->model->getCollider();
    }

    void execute() override { model->setCollider(newCollider); }
    void undo() override { model->setCollider(oldCollider); }

   private:
    std::shared_ptr<Model> model;
    std::shared_ptr<Collider> newCollider;
    std::shared_ptr<Collider> oldCollider;
};
