#include "editorLayout.h"
#include <algorithm>

EditorLayoutRects EditorLayout::compute(const glm::vec2 &displaySize) {
    const float bodyTop = toolbarHeight;
    const float bodyHeight = std::max(displaySize.y - bodyTop, 0.0f);
    const float centerX = hierarchyWidth;
    const float centerWidth =
        std::max(displaySize.x - hierarchyWidth - inspectorWidth, minViewportSize);
    const float viewportHeight = std::max(bodyHeight - libraryHeight, minViewportSize);

    EditorLayoutRects rects;
    rects.toolbar = {.pos = {0.0f, 0.0f}, .size = {displaySize.x, toolbarHeight}};
    rects.hierarchy = {.pos = {0.0f, bodyTop}, .size = {hierarchyWidth, bodyHeight}};
    rects.inspector = {.pos = {centerX + centerWidth, bodyTop},
                       .size = {inspectorWidth, bodyHeight}};
    rects.viewport = {.pos = {centerX, bodyTop}, .size = {centerWidth, viewportHeight}};
    rects.library = {.pos = {centerX, bodyTop + viewportHeight},
                     .size = {centerWidth, std::max(bodyHeight - viewportHeight, 0.0f)}};
    return rects;
}
