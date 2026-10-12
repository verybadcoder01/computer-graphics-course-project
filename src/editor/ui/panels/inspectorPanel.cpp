#include "inspectorPanel.h"
#include <format>
#include "visitor/inspectorVisitor/inspectorVisitor.h"

void InspectorPanel::drawContents(IGuiAdapter &gui, EditorCore &editor) {
    const auto selected = editor.getSelected();
    if (!selected) {
        gui.text("Nothing selected");
        return;
    }

    gui.beginDisabled(!editor.canEdit());

    gui.text(std::format("ID {}", selected->getId()));
    std::string name = selected->getName();
    const std::string nameBefore = name;
    if (gui.inputText("Name", name)) {
        selected->setName(name);
    }
    tracker.track<std::string>(gui, editor, nameBefore, name,
                               [selected](const std::string &v) { selected->setName(v); });

    InspectorVisitor visitor(gui, editor, tracker, selected->getId());
    selected->accept(visitor);

    gui.endDisabled();
}
