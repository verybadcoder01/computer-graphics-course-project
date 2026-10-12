#pragma once

#include "adapters/gui/guiAdapter.h"

struct EditorLayoutRects {
    GuiRect toolbar;
    GuiRect hierarchy;
    GuiRect inspector;
    GuiRect library;
    GuiRect viewport;
};

class EditorLayout {
   public:
    static EditorLayoutRects compute(const glm::vec2 &displaySize);

   private:
    static constexpr float toolbarHeight = 60.0f;
    static constexpr float hierarchyWidth = 240.0f;
    static constexpr float inspectorWidth = 340.0f;
    static constexpr float libraryHeight = 200.0f;
    static constexpr float minViewportSize = 100.0f;
};
