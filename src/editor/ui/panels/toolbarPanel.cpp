#include "toolbarPanel.h"
#include <format>
#include "visitor/particleCountVisitor/particleCountVisitor.h"

void ToolbarPanel::drawContents(IGuiAdapter &gui, EditorCore &editor) {
    const bool editing = editor.getMode() == EditorMode::Edit;

    gui.beginDisabled(editing);
    if (gui.button("Edit")) {
        editor.setMode(EditorMode::Edit);
    }
    gui.endDisabled();
    gui.sameLine();
    gui.beginDisabled(!editing);
    if (gui.button("Simulate")) {
        editor.setMode(EditorMode::Simulation);
    }
    gui.endDisabled();

    if (!editing) {
        gui.sameLine();
        if (gui.button(editor.isPaused() ? "Resume" : "Pause")) {
            editor.setPaused(!editor.isPaused());
        }
    }

    gui.sameLine();
    gui.text("|");
    gui.sameLine();
    gui.beginDisabled(!editor.canUndo());
    if (gui.button("Undo")) {
        editor.undo();
    }
    gui.endDisabled();
    gui.sameLine();
    gui.beginDisabled(!editor.canRedo());
    if (gui.button("Redo")) {
        editor.redo();
    }
    gui.endDisabled();

    ParticleCountVisitor counter;
    editor.getContext()->scene->accept(counter);
    const float frameTime = editor.getLastFrameTime();
    const float fps = frameTime > 0.0f ? 1.0f / frameTime : 0.0f;
    gui.sameLine();
    gui.text(std::format("| {:.0f} FPS | {} particles", fps, counter.getCount()));

    if (!editor.getStatusMessage().empty()) {
        gui.sameLine();
        gui.textColored({1.0f, 0.4f, 0.4f, 1.0f}, editor.getStatusMessage());
    }
}
