#pragma once
#include <memory>
#include <vector>

class ICommand {
   public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class CommandHistory {
   public:
    explicit CommandHistory(size_t maxHistorySize = 100) : maxHistorySize(maxHistorySize) {}

    void executeCommand(std::unique_ptr<ICommand> command) {
        if (!command)
            return;

        command->execute();
        undoStack.push_back(std::move(command));
        redoStack.clear();

        if (undoStack.size() > maxHistorySize) {
            undoStack.erase(undoStack.begin());
        }
    }

    void undo() {
        if (undoStack.empty())
            return;

        auto command = std::move(undoStack.back());
        undoStack.pop_back();

        command->undo();
        redoStack.push_back(std::move(command));
    }

    void redo() {
        if (redoStack.empty())
            return;

        auto command = std::move(redoStack.back());
        redoStack.pop_back();

        command->execute();
        undoStack.push_back(std::move(command));
    }

    bool canUndo() const noexcept { return !undoStack.empty(); }
    bool canRedo() const noexcept { return !redoStack.empty(); }
    void clear() {
        undoStack.clear();
        redoStack.clear();
    }

   private:
    std::vector<std::unique_ptr<ICommand>> undoStack;
    std::vector<std::unique_ptr<ICommand>> redoStack;
    size_t maxHistorySize;
};
