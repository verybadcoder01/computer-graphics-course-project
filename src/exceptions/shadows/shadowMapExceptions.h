#pragma once

#include <source_location>
#include "exceptions/baseException.h"

class ShadowMapException : public BaseException {
   public:
    ShadowMapException(const std::string &info, const std::source_location &loc) noexcept
        : BaseException(info, loc) {}
};

class ShadowFramebufferIncomplete : public ShadowMapException {
   public:
    ShadowFramebufferIncomplete(const std::string &info, const std::source_location &loc) noexcept
        : ShadowMapException(info, loc) {}
};