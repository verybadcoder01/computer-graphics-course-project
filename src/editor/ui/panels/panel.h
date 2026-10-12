#pragma once

#include <string>
#include "adapters/gui/guiAdapter.h"
#include "editor/core/editorCore.h"

class IPanel {
   public:
    virtual ~IPanel() = default;

    virtual std::string getTitle() const = 0;
    virtual void drawContents(IGuiAdapter &gui, EditorCore &editor) = 0;
};
