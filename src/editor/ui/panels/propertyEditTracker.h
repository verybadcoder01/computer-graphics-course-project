#pragma once

#include <any>
#include <memory>
#include "adapters/gui/guiAdapter.h"
#include "editor/command/commands/setPropertyCommand.h"
#include "editor/core/editorCore.h"

// Turns live edits into undoable commands. Continuous widgets (drags, colors, text) apply their
// value to the object every frame; track() remembers the value from before the edit started and
// records one command when the widget is released. Discrete widgets (combo, checkbox) use commit().
class PropertyEditTracker {
   public:
    template <typename T>
    void track(IGuiAdapter &gui,
               EditorCore &editor,
               const T &before,
               const T &after,
               typename SetPropertyCommand<T>::Setter setter) {
        if (gui.isItemActivated()) {
            valueBeforeEdit = before;
        }
        if (gui.isItemDeactivatedAfterEdit()) {
            if (const T *old = std::any_cast<T>(&valueBeforeEdit)) {
                editor.executeCommand(
                    std::make_unique<SetPropertyCommand<T>>(std::move(setter), *old, after));
            }
            valueBeforeEdit.reset();
        }
    }

    template <typename T>
    static void commit(EditorCore &editor,
                       const T &before,
                       const T &after,
                       typename SetPropertyCommand<T>::Setter setter) {
        editor.executeCommand(
            std::make_unique<SetPropertyCommand<T>>(std::move(setter), before, after));
    }

   private:
    std::any valueBeforeEdit;
};
