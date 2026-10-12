#pragma once

#include "adapters/gui/guiAdapter.h"
#include "editor/core/editorCore.h"
#include "editor/ui/panels/hierarchyPanel.h"
#include "editor/ui/panels/inspectorPanel.h"
#include "editor/ui/panels/modelLibraryPanel.h"
#include "editor/ui/panels/toolbarPanel.h"
#include "editor/ui/panels/viewportPanel.h"

class EditorUI {
   public:
    void draw(IGuiAdapter &gui, EditorCore &editor);

   private:
    ToolbarPanel toolbar;
    HierarchyPanel hierarchy;
    InspectorPanel inspector;
    ModelLibraryPanel library;
    ViewportPanel viewport;

    static void drawPanel(IGuiAdapter &gui, EditorCore &editor, IPanel &panel, const GuiRect &rect);
};
