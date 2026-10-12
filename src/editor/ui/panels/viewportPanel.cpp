#include "viewportPanel.h"

void ViewportPanel::drawContents(IGuiAdapter &gui, EditorCore &editor) {
    Viewport &viewport = editor.getViewport();
    const glm::vec2 size = gui.getContentRegionAvail();
    // resize before taking the texture id: resizing recreates the texture
    viewport.resize(static_cast<int>(size.x), static_cast<int>(size.y));
    gui.image(viewport.getColorTexture(), size, true);

    const bool hovered = gui.isItemHovered();
    editor.getContext()->viewportHovered = hovered;

    const InputState input = gui.getInput();
    if (hovered && input.isClicked(MouseButton::Left) && !input.isDown(MouseButton::Right)) {
        const glm::vec2 local = input.mousePos - gui.getItemRect().pos;
        viewport.requestPick(static_cast<int>(local.x), static_cast<int>(local.y));
    }
}
