#pragma once

#include <utility>
#include "shaders/shader.h"
class ViewportFramebuffer {
   public:
    ViewportFramebuffer(int width, int height);
    virtual ~ViewportFramebuffer();

    ViewportFramebuffer(const ViewportFramebuffer &) = delete;
    ViewportFramebuffer &operator=(const ViewportFramebuffer &) = delete;
    ViewportFramebuffer(ViewportFramebuffer &&other) noexcept
        : width(std::exchange(other.width, 0)),
          height(std::exchange(other.height, 0)),
          fbo(std::exchange(other.fbo, 0)),
          colorTexture(std::exchange(other.colorTexture, 0)),
          rboDepth(std::exchange(other.rboDepth, 0)) {}
    ViewportFramebuffer &operator=(ViewportFramebuffer &&other) noexcept {
        if (this != &other) {
            width = std::exchange(other.width, 0);
            height = std::exchange(other.height, 0);
            fbo = std::exchange(other.fbo, 0);
            colorTexture = std::exchange(other.colorTexture, 0);
            rboDepth = std::exchange(other.rboDepth, 0);
        }
        return *this;
    }

    void bind() const;
    void unbind() const;
    void resize(int newWidth, int newHeight);
    void fillUint(uint32_t value);

    GLuint getColorTexture() const noexcept { return colorTexture; }
    GLuint getFBO() const noexcept { return fbo; }
    int getWidth() const noexcept { return width; }
    int getHeight() const noexcept { return height; }

   protected:
    int width, height;
    GLuint fbo = 0;
    GLuint colorTexture = 0;
    GLuint rboDepth = 0;

    void init();
    void cleanup();
};