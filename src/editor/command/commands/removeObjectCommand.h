#pragma once

#include <memory>
#include "editor/command/commandInterface.h"
#include "editor/selectionManager/selectionManager.h"
#include "scene/scene.h"

class RemoveObjectCommand : public ICommand {
   public:
    RemoveObjectCommand(std::shared_ptr<Scene> scene,
                        std::shared_ptr<SelectionManager> selection,
                        std::shared_ptr<BaseObject> object);

    void execute() override;
    void undo() override;

   private:
    std::shared_ptr<Scene> scene;
    std::shared_ptr<SelectionManager> selection;
    std::shared_ptr<BaseObject> object;
};
