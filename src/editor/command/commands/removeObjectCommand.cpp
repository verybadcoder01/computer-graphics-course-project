#include "removeObjectCommand.h"

RemoveObjectCommand::RemoveObjectCommand(std::shared_ptr<Scene> scene,
                                         std::shared_ptr<SelectionManager> selection,
                                         std::shared_ptr<BaseObject> object)
    : scene(std::move(scene)), selection(std::move(selection)), object(std::move(object)) {}

void RemoveObjectCommand::execute() {
    if (selection->getSelected() == object) {
        selection->clearSelection();
    }
    scene->removeObject(object->getId());
}

void RemoveObjectCommand::undo() {
    scene->addObject(object);
    selection->select(object);
}
