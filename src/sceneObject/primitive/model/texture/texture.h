#pragma once

#include <glad/glad.h>
#include <filesystem>
#include "glm/glm.hpp"

class Texture {
   public:
    Texture() = default;
    explicit Texture(const std::filesystem::path &filepath);
    static Texture createSolidColor(const glm::u8vec4 &rgba);
    ~Texture();

    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;

    Texture(Texture &&other) noexcept;
    Texture &operator=(Texture &&other) noexcept;

    void bind(uint32_t slot = 0) const;
    void unbind() const;

    GLuint getId() const noexcept { return id; }
    bool isLoaded() const noexcept { return id != 0; }

   private:
    GLuint id{0};
    void cleanup() noexcept;
};
