#pragma once

#include "sceneObject/primitive/model/texture/texture.h"
#include "sceneObject/primitive/primitive.h"
#include <cstddef>

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

    const Texture* getTexture() const noexcept;

    size_t getIndexCount() const noexcept;
    size_t getIndexDataSize() const noexcept;
    const void* getRawIndexData() const;
    size_t getVertexDataSize() const noexcept;
    const void* getRawVertexData() const;
    size_t getVertexStride() const noexcept;
};
