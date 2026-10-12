#pragma once

#include "editor/ui/panels/panel.h"

class ModelLibraryPanel : public IPanel {
   public:
    std::string getTitle() const override { return "Model library"; }
    void drawContents(IGuiAdapter &gui, EditorCore &editor) override;
};
