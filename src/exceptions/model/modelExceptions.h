#pragma once

#include "exceptions/baseException.h"
#include <source_location>

class ModelException : public BaseException {
public:
    ModelException(const std::string &info, const std::source_location &loc) noexcept: BaseException(info, loc) {}
};

class FailedToLoadModelException: public ModelException {
public:
    FailedToLoadModelException(const std::string &info, const std::source_location &loc) noexcept: ModelException(info, loc) {}
};