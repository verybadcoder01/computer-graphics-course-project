#pragma once

#include "editor/ui/panels/panel.h"

class ToolbarPanel : public IPanel {
   public:
    std::string getTitle() const override { return "Toolbar"; }
    void drawContents(IGuiAdapter &gui, EditorCore &editor) override;
};
