#include "editorUI.h"
#include "editor/ui/editorLayout.h"

void EditorUI::draw(IGuiAdapter &gui, EditorCore &editor) {
    const EditorLayoutRects rects = EditorLayout::compute(gui.getDisplaySize());
    drawPanel(gui, editor, toolbar, rects.toolbar);
    drawPanel(gui, editor, hierarchy, rects.hierarchy);
    drawPanel(gui, editor, inspector, rects.inspector);
    drawPanel(gui, editor, library, rects.library);
    drawPanel(gui, editor, viewport, rects.viewport);
}

void EditorUI::drawPanel(IGuiAdapter &gui, EditorCore &editor, IPanel &panel, const GuiRect &rect) {
    if (gui.beginPanel(panel.getTitle(), rect)) {
        panel.drawContents(gui, editor);
    }
    gui.endPanel();
}
