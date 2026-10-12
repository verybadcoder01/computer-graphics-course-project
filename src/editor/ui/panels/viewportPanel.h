#pragma once

#include "editor/ui/panels/panel.h"

class ViewportPanel : public IPanel {
   public:
    std::string getTitle() const override { return "Viewport"; }
    void drawContents(IGuiAdapter &gui, EditorCore &editor) override;
};
