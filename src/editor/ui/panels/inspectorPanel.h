#pragma once

#include "editor/ui/panels/panel.h"
#include "editor/ui/panels/propertyEditTracker.h"

class InspectorPanel : public IPanel {
   public:
    std::string getTitle() const override { return "Inspector"; }
    void drawContents(IGuiAdapter &gui, EditorCore &editor) override;

   private:
    PropertyEditTracker tracker;
};
