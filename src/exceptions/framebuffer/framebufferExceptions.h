#pragma once

#include <source_location>
#include "exceptions/baseException.h"

class FramebufferException : public BaseException {
   public:
    FramebufferException(const std::string &info, const std::source_location &loc) noexcept
        : BaseException(info, loc) {}
};

class FramebufferNotReadyException : public FramebufferException {
   public:
    FramebufferNotReadyException(const std::string &info, const std::source_location &loc) noexcept
        : FramebufferException(info, loc) {}
};