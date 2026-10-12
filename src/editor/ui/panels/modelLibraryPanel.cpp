#include "modelLibraryPanel.h"
#include <format>
#include <optional>

void ModelLibraryPanel::drawContents(IGuiAdapter &gui, EditorCore &editor) {
    ModelLibrary &library = editor.getModelLibrary();

    if (gui.button("Rescan")) {
        library.scan();
    }
    gui.sameLine();
    gui.text(library.getRoot().string());

    if (library.getEntries().empty()) {
        gui.text(std::format("No models found. Put .obj files into {}/parts and {}/nozzles",
                             library.getRoot().string(), library.getRoot().string()));
        return;
    }

    std::optional<LibraryEntry> toSpawn;
    gui.beginDisabled(!editor.canEdit());
    for (const auto kind : {LibraryEntryKind::Part, LibraryEntryKind::Nozzle}) {
        gui.separator();
        gui.text(kind == LibraryEntryKind::Part ? "Parts" : "Nozzles (emitters)");
        for (const auto &entry : library.getEntries()) {
            if (entry.kind != kind) {
                continue;
            }
            if (gui.button(std::format("Add##{}", entry.objPath.string()))) {
                toSpawn = entry;
            }
            gui.sameLine();
            gui.text(entry.texturePath.empty() ? entry.name : entry.name + " (textured)");
        }
    }
    gui.endDisabled();

    if (toSpawn) {
        editor.spawnFromLibrary(*toSpawn);
    }
}
