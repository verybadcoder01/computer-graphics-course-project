#include "hierarchyPanel.h"
#include <format>

void HierarchyPanel::drawContents(IGuiAdapter &gui, EditorCore &editor) {
    const auto selected = editor.getSelected();

    for (const auto &[id, object] : editor.getContext()->scene->getObjects()) {
        const std::string name = object->getName().empty() ? "Object" : object->getName();
        if (gui.selectable(std::format("{} #{}##{}", name, id, id), object == selected)) {
            editor.select(object);
        }
    }

    gui.separator();
    gui.beginDisabled(!editor.isRemovable(selected));
    if (gui.button("Delete")) {
        editor.removeSelected();
    }
    gui.endDisabled();
}
