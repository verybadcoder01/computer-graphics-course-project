#include "shadowMap.h"
#include "exceptions/shadows/shadowMapExceptions.h"
#include <cstdint>
#include <utility>

ShadowMap::ShadowMap(uint32_t width, uint32_t height): width(width), height(height) {
    glGenFramebuffers(1, &fbo);
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        cleanup();
        throw ShadowFramebufferIncomplete(
            std::format("Shadow Framebuffer is not complete! Resolution: {}x{}", width, height),
            std::source_location::current()
        );
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShadowMap::~ShadowMap() { 
    cleanup();
}

ShadowMap::ShadowMap(ShadowMap &&other) noexcept: 
    fbo(std::exchange(other.fbo, 0)), depthTexture(std::exchange(other.depthTexture, 0)),
    width(std::exchange(other.width, 0)), height(std::exchange(other.height, 0)) {}

ShadowMap& ShadowMap::operator=(ShadowMap&& other) noexcept {
    if (this != &other) {
        cleanup();
        fbo = std::exchange(other.fbo, 0);
        depthTexture = std::exchange(other.depthTexture, 0);
        width = std::exchange(other.width, 0);
        height = std::exchange(other.height, 0);
    }
    return *this;
}

void ShadowMap::cleanup() noexcept {
    if (depthTexture != 0) {
        glDeleteTextures(1, &depthTexture);
        depthTexture = 0;
    }
    if (fbo != 0){
        glDeleteFramebuffers(1, &fbo);
        fbo = 0;
    }
}

void ShadowMap::bind() const noexcept {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void ShadowMap::unbind() const noexcept {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::bindTexture(uint32_t slot) const noexcept {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
}
