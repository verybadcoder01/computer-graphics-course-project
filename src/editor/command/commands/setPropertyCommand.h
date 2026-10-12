#pragma once

#include <functional>
#include "editor/command/commandInterface.h"

// the setter captures the edited object (as a shared_ptr), so the command keeps it alive
template <typename T>
class SetPropertyCommand : public ICommand {
   public:
    using Setter = std::function<void(const T &)>;

    SetPropertyCommand(Setter setter, T oldValue, T newValue)
        : setter(std::move(setter)), oldValue(std::move(oldValue)), newValue(std::move(newValue)) {}

    void execute() override { setter(newValue); }
    void undo() override { setter(oldValue); }

   private:
    Setter setter;
    T oldValue;
    T newValue;
};
