#pragma once

#include <cstddef>
#include <vector>
#include "sceneObject/collider/baseCollider.h"
#include "sceneObject/primitive/model/texture/texture.h"
#include "sceneObject/primitive/primitive.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;

    static constexpr size_t getPositionOffset() noexcept { return offsetof(Vertex, position); }
    static constexpr size_t getNormalOffset() noexcept { return offsetof(Vertex, normal); }
    static constexpr size_t getTexCoordOffset() noexcept { return offsetof(Vertex, texCoord); }
};

class Model : public Primitive {
   public:
    virtual ~Model() override = default;

    virtual const Texture *getTexture() const noexcept = 0;
    virtual void setCollider(std::shared_ptr<Collider> coll) noexcept = 0;
    virtual std::shared_ptr<Collider> getCollider() const = 0;

    virtual const std::vector<Vertex> &getVertices() const noexcept = 0;

    virtual size_t getIndexCount() const noexcept = 0;
    virtual size_t getIndexDataSize() const noexcept = 0;
    virtual const void *getRawIndexData() const = 0;
    virtual size_t getVertexDataSize() const noexcept = 0;
    virtual const void *getRawVertexData() const = 0;
    virtual size_t getVertexStride() const noexcept = 0;
};
