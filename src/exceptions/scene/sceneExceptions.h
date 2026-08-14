#pragma once

#include "exceptions/baseException.h"
#include <source_location>

class SceneException : public BaseException {
public:
    SceneException(const std::string &info, const std::source_location &loc) noexcept: BaseException(info, loc) {}
};

class ObjectKeyNotFoundException: public SceneException {
public:
    ObjectKeyNotFoundException(const std::string &info, const std::source_location &loc) noexcept: SceneException(info, loc) {}
};