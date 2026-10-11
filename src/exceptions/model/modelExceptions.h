#pragma once

#include <source_location>
#include "exceptions/baseException.h"

class ModelException : public BaseException {
   public:
    ModelException(const std::string &info, const std::source_location &loc) noexcept
        : BaseException(info, loc) {}
};

class FailedToLoadModelException : public ModelException {
   public:
    FailedToLoadModelException(const std::string &info, const std::source_location &loc) noexcept
        : ModelException(info, loc) {}
};