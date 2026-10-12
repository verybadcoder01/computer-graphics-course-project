#include "shaders/shader.h"
#include <fstream>
#include <source_location>
#include <sstream>
#include "exceptions/shaders/shaderExceptions.h"
#include "glm/gtc/type_ptr.hpp"

Shader::Shader(const std::filesystem::path &vertexPath, const std::filesystem::path &fragmentPath) {
    loadFromFiles(vertexPath, fragmentPath);
}

Shader::~Shader() {
    cleanup();
}

Shader::Shader(Shader &&other) noexcept : programId(other.programId) {
    other.programId = 0;
}

Shader &Shader::operator=(Shader &&other) noexcept {
    if (this != &other) {
        cleanup();
        programId = other.programId;
        other.programId = 0;
    }
    return *this;
}

void Shader::cleanup() noexcept {
    if (programId != 0) {
        glDeleteProgram(programId);
        programId = 0;
    }
}

void Shader::loadFromFiles(const std::filesystem::path &vertexPath,
                           const std::filesystem::path &fragmentPath) {
    cleanup();
    std::string vertexCode, fragmentCode;
    std::ifstream vShaderFile, fShaderFile;
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    std::stringstream vShaderStream, fShaderStream;

    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (const std::ifstream::failure &e) {
        throw FailedToReadShaderSourceException(
            std::format("Failed to read shader source file: {}", e.what()),
            std::source_location::current());
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    GLuint vertex = 0, fragment = 0;
    try {
        vertex = compileShader(CompileType::VERTEX, vShaderCode);
        checkCompilerErrors(vertex, CompileType::VERTEX);
        fragment = compileShader(CompileType::FRAGMENT, fShaderCode);
        checkCompilerErrors(fragment, CompileType::FRAGMENT);
        createAndLinkProgram(vertex, fragment);
        checkCompilerErrors(programId, CompileType::PROGRAM);
    } catch (...) {
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        cleanup();
        throw;
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

GLuint Shader::compileShader(CompileType shaderType, const char *shaderCode) {
    GLuint shader = 0;
    if (shaderType == VERTEX) {
        shader = glCreateShader(GL_VERTEX_SHADER);
    } else if (shaderType == FRAGMENT) {
        shader = glCreateShader(GL_FRAGMENT_SHADER);
    } else {
        throw UnknownShaderTypeException(std::format("Unknown shader type {}", (int)shaderType),
                                         std::source_location::current());
    }
    glShaderSource(shader, 1, &shaderCode, nullptr);
    glCompileShader(shader);
    return shader;
}

void Shader::createAndLinkProgram(GLuint vertexShader, GLuint fragmentShader) {
    programId = glCreateProgram();
    glAttachShader(programId, vertexShader);
    glAttachShader(programId, fragmentShader);
    glLinkProgram(programId);
}

void Shader::use() const {
    glUseProgram(programId);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(programId, name.c_str()), static_cast<int>(value));
}

void Shader::setUInt(const std::string &name, uint32_t value) const {
    glUniform1ui(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(programId, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(programId, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE,
                       glm::value_ptr(mat));
}

void Shader::checkCompilerErrors(GLuint shader, CompileType type) {
    GLint success;
    const size_t infoLogSize = 1024;
    GLchar infoLog[infoLogSize];
    if (type != CompileType::PROGRAM) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, infoLogSize, nullptr, infoLog);
            throw ShaderCompilationFailedException(
                std::format("Failed to compile shader of type {}: {}", (int)type, infoLog),
                std::source_location::current());
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, infoLogSize, nullptr, infoLog);
            throw ShaderLinkingFailedException(
                std::format("Failed to link shader program: {}", infoLog),
                std::source_location::current());
        }
    }
}
