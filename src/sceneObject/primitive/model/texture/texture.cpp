#include "texture.h"
#include <format>
#include <source_location>
#include <utility>
#include "exceptions/model/modelExceptions.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::filesystem::path &filepath) {
    stbi_set_flip_vertically_on_load(true);

    int width = 0, height = 0, nrChannels = 0;
    unsigned char *data = stbi_load(filepath.string().c_str(), &width, &height, &nrChannels, 0);

    if (!data) {
        throw FailedToLoadModelException(
            std::format("Failed to load texture file: {}", filepath.string()),
            std::source_location::current());
    }

    GLenum format = GL_RGB;
    if (nrChannels == 1) {
        format = GL_RED;
    } else if (nrChannels == 3) {
        format = GL_RGB;
    } else if (nrChannels == 4) {
        format = GL_RGBA;
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture Texture::createSolidColor(const glm::u8vec4 &rgba) {
    Texture texture;
    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &rgba.x);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

Texture::~Texture() {
    cleanup();
}

Texture::Texture(Texture &&other) noexcept : id(std::exchange(other.id, 0)) {}

Texture &Texture::operator=(Texture &&other) noexcept {
    if (this != &other) {
        cleanup();
        id = std::exchange(other.id, 0);
    }
    return *this;
}

void Texture::cleanup() noexcept {
    if (id != 0) {
        glDeleteTextures(1, &id);
        id = 0;
    }
}

void Texture::bind(uint32_t slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}
