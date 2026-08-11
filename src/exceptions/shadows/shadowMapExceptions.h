#pragma once

#include "exceptions/baseException.h"
#include <source_location>

class ShadowMapException : public BaseException {
public:
    ShadowMapException(const std::string &info, const std::source_location &loc) noexcept: BaseException(info, loc) {}
};

class ShadowFramebufferIncomplete: public ShadowMapException {
public:
    ShadowFramebufferIncomplete(const std::string &info, const std::source_location &loc) noexcept: ShadowMapException(info, loc) {}
};