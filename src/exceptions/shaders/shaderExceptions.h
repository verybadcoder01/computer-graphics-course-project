#pragma once

#include "exceptions/baseException.h"
#include <source_location>

class ShaderException : public BaseException {
public:
    ShaderException(const std::string &info, const std::source_location &loc) noexcept: BaseException(info, loc) {}
};

class FailedToReadShaderSourceException: public ShaderException {
public:
    FailedToReadShaderSourceException(const std::string &info, const std::source_location &loc) noexcept: ShaderException(info, loc) {}
};

class UnknownShaderTypeException: public ShaderException {
public:
    UnknownShaderTypeException(const std::string &info, const std::source_location &loc) noexcept: ShaderException(info, loc) {}
};

class ShaderCompilationFailedException: public ShaderException {
public:
    ShaderCompilationFailedException(const std::string &info, const std::source_location &loc) noexcept: ShaderException(info, loc) {}
};

class ShaderLinkingFailedException: public ShaderException {
public:
    ShaderLinkingFailedException(const std::string &info, const std::source_location &loc) noexcept: ShaderException(info, loc) {}
};
