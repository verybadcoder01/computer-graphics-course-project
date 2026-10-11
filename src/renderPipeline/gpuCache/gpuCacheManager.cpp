#include "gpuCacheManager.h"

void GPUBufferHandles::clear() {
    deleteEBO();
    deleteVBO();
    deleteVAO();
    deleteInstanceVBO();
}

void GPUBufferHandles::deleteEBO() {
    if (EBO) {
        glDeleteBuffers(1, &EBO);
    }
}

void GPUBufferHandles::deleteVBO() {
    if (VBO) {
        glDeleteBuffers(1, &VBO);
    }
}

void GPUBufferHandles::deleteVAO() {
    if (VAO) {
        glDeleteVertexArrays(1, &VAO);
    }
}

void GPUBufferHandles::deleteInstanceVBO() {
    if (instanceVBO) {
        glDeleteBuffers(1, &instanceVBO);
    }
}

void GPUCacheManager::cleanupExpired() {
    for (auto it = gpuCache.begin(); it != gpuCache.end();) {
        if (it->first.expired()) {
            it = gpuCache.erase(it);
        } else {
            ++it;
        }
    }
}

void GPUCacheManager::clearAll() noexcept {
    gpuCache.clear();
}
