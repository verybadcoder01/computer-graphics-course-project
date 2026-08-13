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
    if (instanceVBO){
        glDeleteBuffers(1, &instanceVBO);
    }
}

template<typename ObjectType>
const GPUBufferHandles& GPUCacheManager::getOrUpload(const ObjectType &obj, std::function<GPUBufferHandles(const ObjectType&)> upload) {
    std::weak_ptr<const BaseObject> key = obj.shared_from_this();
    auto it = gpuCache.find(key);

    if (it == gpuCache.end()) {
        GPUBufferHandles handles = upload(obj);
        auto [insertedIt, success] = gpuCache.emplace(key, std::move(handles));
        it = insertedIt;
    }
    return it->second;
}

void GPUCacheManager::cleanupExpired() {
    for (auto it = gpuCache.begin(); it != gpuCache.end(); ) {
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
