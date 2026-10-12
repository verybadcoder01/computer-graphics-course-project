#pragma once

#include <source_location>
#include "exceptions/baseException.h"

class WindowException : public BaseException {
   public:
    WindowException(const std::string &info, const std::source_location &loc) noexcept
        : BaseException(info, loc) {}
};

class WindowCreationException : public WindowException {
   public:
    WindowCreationException(const std::string &info, const std::source_location &loc) noexcept
        : WindowException(info, loc) {}
};
