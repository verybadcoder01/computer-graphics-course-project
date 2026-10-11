#pragma once

#include <glad/glad.h>
#include <cstdint>

class ShadowMap {
   public:
    ShadowMap() = default;
    ShadowMap(uint32_t width, uint32_t height);
    virtual ~ShadowMap();

    ShadowMap(const ShadowMap &) = delete;
    ShadowMap &operator=(const ShadowMap &) = delete;
    ShadowMap(ShadowMap &&other) noexcept;
    ShadowMap &operator=(ShadowMap &&other) noexcept;

    void bind() const noexcept;
    void unbind() const noexcept;
    void bindTexture(uint32_t slot = 1) const noexcept;

    GLuint getFBO() const noexcept { return fbo; }
    GLuint getDepthTexture() const noexcept { return depthTexture; }
    uint32_t getWidth() const noexcept { return width; }
    uint32_t getHeight() const noexcept { return height; }

   protected:
    void cleanup() noexcept;

    GLuint fbo = 0;
    GLuint depthTexture = 0;
    uint32_t width = 0;
    uint32_t height = 0;
};
