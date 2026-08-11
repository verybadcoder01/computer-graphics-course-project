#pragma once

#include "sceneObject/baseObject.h"
#include "glad/glad.h"
#include <map>

struct GPUBufferHandles {
    ~GPUBufferHandles() {
        clear();
    }

    GPUBufferHandles() = default;
    GPUBufferHandles(const GPUBufferHandles&) = delete;
    GPUBufferHandles& operator=(const GPUBufferHandles&) = delete;
    GPUBufferHandles(GPUBufferHandles&& other) noexcept : VAO(other.VAO), VBO(other.VBO), EBO(other.EBO), instanceVBO(other.instanceVBO), elementCount(other.elementCount) {
        other.VAO = 0;
        other.VBO = 0;
        other.EBO = 0;
        other.instanceVBO = 0;
        other.elementCount = 0;
    }

    GPUBufferHandles& operator=(GPUBufferHandles&& other) noexcept {
        if (this != &other) {
            clear();
            VAO = other.VAO; 
            VBO = other.VBO; 
            EBO = other.EBO; 
            instanceVBO = other.instanceVBO;
            elementCount = other.elementCount;

            other.VAO = 0; other.VBO = 0; other.EBO = 0; other.elementCount = 0;
            other.instanceVBO = 0;
        }
        return *this;
    }

    GLuint getVAO() const { return VAO; }
    GLuint& getVAO() { return VAO; }
    GLuint getVBO() const { return VBO; }
    GLuint& getVBO() { return VBO; }
    GLuint getEBO() const { return EBO; }
    GLuint& getEBO() { return EBO; }
    GLuint getInstanceVBO() const { return instanceVBO; }
    GLuint& getInstanceVBO() { return instanceVBO; }
    GLsizei getElementCount() const { return elementCount; }
    GLsizei& getElementCount() { return elementCount; }

private:
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;
    GLuint instanceVBO = 0;
    GLsizei elementCount = 0;

    void clear();
    void deleteEBO();
    void deleteVBO();
    void deleteVAO();
    void deleteInstanceVBO();
};

class GPUCacheManager {
public:
    GPUCacheManager() = default;
    ~GPUCacheManager() = default;

    GPUCacheManager(const GPUCacheManager&) = delete;
    GPUCacheManager& operator=(const GPUCacheManager&) = delete;

    template<typename ObjectType, typename UploadFunc>
    const GPUBufferHandles& getOrUpload(ObjectType &obj, UploadFunc upload);
    void cleanupExpired();

    void clearAll() noexcept;

private:
    std::map<std::weak_ptr<const BaseObject>, GPUBufferHandles, std::owner_less<std::weak_ptr<const BaseObject>>> gpuCache;
};