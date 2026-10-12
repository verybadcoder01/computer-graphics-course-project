#pragma once

#include <source_location>
#include "exceptions/baseException.h"

class ColliderException : public BaseException {
   public:
    ColliderException(const std::string &info, const std::source_location &loc) noexcept
        : BaseException(info, loc) {}
};

class InvalidColliderGeometryException : public ColliderException {
   public:
    InvalidColliderGeometryException(const std::string &info,
                                     const std::source_location &loc) noexcept
        : ColliderException(info, loc) {}
};
