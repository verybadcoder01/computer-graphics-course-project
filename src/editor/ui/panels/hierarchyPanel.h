#pragma once

#include "editor/ui/panels/panel.h"

class HierarchyPanel : public IPanel {
   public:
    std::string getTitle() const override { return "Scene"; }
    void drawContents(IGuiAdapter &gui, EditorCore &editor) override;
};
