#include "viewportFramebuffer.h"
#include <source_location>
#include "exceptions/framebuffer/framebufferExceptions.h"
#include "shaders/shader.h"

ViewportFramebuffer::ViewportFramebuffer(int width, int height, FramebufferColorFormat colorFormat)
    : width(width), height(height), colorFormat(colorFormat) {
    init();
}

ViewportFramebuffer::~ViewportFramebuffer() {
    cleanup();
}

void ViewportFramebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, width, height);
}

void ViewportFramebuffer::unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ViewportFramebuffer::resize(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
    cleanup();
    init();
}

void ViewportFramebuffer::fillUint(uint32_t value) {
    const GLuint clearValue[4] = {value, 0, 0, 0};
    glClearBufferuiv(GL_COLOR, 0, clearValue);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void ViewportFramebuffer::init() {
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    GLint filter = GL_LINEAR;
    if (colorFormat == FramebufferColorFormat::R32UI) {
        // integer textures can't be filtered
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT,
                     nullptr);
        filter = GL_NEAREST;
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     nullptr);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                              rboDepth);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        cleanup();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        throw FramebufferNotReadyException("Viewport Framebuffer is not complete!",
                                           std::source_location::current());
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ViewportFramebuffer::cleanup() noexcept {
    if (fbo) {
        glDeleteFramebuffers(1, &fbo);
        fbo = 0;
    }
    if (colorTexture) {
        glDeleteTextures(1, &colorTexture);
        colorTexture = 0;
    }
    if (rboDepth) {
        glDeleteRenderbuffers(1, &rboDepth);
        rboDepth = 0;
    }
}
