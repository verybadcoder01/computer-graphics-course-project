#include "addObjectCommand.h"

AddObjectCommand::AddObjectCommand(std::shared_ptr<Scene> scene,
                                   std::shared_ptr<SelectionManager> selection,
                                   std::shared_ptr<BaseObject> object)
    : scene(std::move(scene)), selection(std::move(selection)), object(std::move(object)) {}

void AddObjectCommand::execute() {
    scene->addObject(object);
    selection->select(object);
}

void AddObjectCommand::undo() {
    if (selection->getSelected() == object) {
        selection->clearSelection();
    }
    scene->removeObject(object->getId());
}
