#pragma once

#include <filesystem>
#include <string>
#include "glad/glad.h"
#include "glm/glm.hpp"

enum CompileType { VERTEX = 0, FRAGMENT, PROGRAM };

class Shader {
   public:
    Shader() = default;
    Shader(const std::filesystem::path &vertexPath, const std::filesystem::path &fragmentPath);
    virtual ~Shader();

    Shader(const Shader &other) = delete;
    Shader &operator=(const Shader &other) = delete;
    Shader(Shader &&other) noexcept;
    Shader &operator=(Shader &&other) noexcept;

    void loadFromFiles(const std::filesystem::path &vertexPath,
                       const std::filesystem::path &fragmentPath);
    GLuint compileShader(CompileType shaderType, const char *shaderCode);
    void createAndLinkProgram(GLuint vertexShader, GLuint fragmentShader);

    void use() const;
    void setBool(const std::string &name, bool value) const;
    void setUInt(const std::string &name, uint32_t value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

    GLuint getID() const noexcept { return programId; }

   protected:
    GLuint programId = 0;

    void cleanup() noexcept;
    void checkCompilerErrors(GLuint shader, CompileType type);
};
