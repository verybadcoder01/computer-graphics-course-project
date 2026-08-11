#pragma once

#include <exception>
#include <string>
#include <source_location>
#include <format>

class BaseException: public std::exception {
public:
    BaseException(const std::string &error, const std::source_location &loc) noexcept {
        msg = std::format("File {} at ({}:{}): {}: {}\n", loc.file_name(), loc.line(), loc.column(), loc.function_name(), error);
    }

    virtual const char* what() const noexcept override {
        return msg.c_str();
    }
protected:
    std::string msg;
};
