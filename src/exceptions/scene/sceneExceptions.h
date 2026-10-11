#pragma once

#include <source_location>
#include "exceptions/baseException.h"

class SceneException : public BaseException {
   public:
    SceneException(const std::string &info, const std::source_location &loc) noexcept
        : BaseException(info, loc) {}
};

class ObjectKeyNotFoundException : public SceneException {
   public:
    ObjectKeyNotFoundException(const std::string &info, const std::source_location &loc) noexcept
        : SceneException(info, loc) {}
};