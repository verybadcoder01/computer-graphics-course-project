#pragma once

#include "sceneObject/primitive/model/model.h"
#include "visitor/objectVisitor.h"
#include "sceneObject/primitive/model/texture/texture.h"
#include <filesystem>
#include <memory>

class ObjModel : public Model {
public:
    ObjModel() = default;
    ObjModel(const std::string &name, 
             const std::filesystem::path &filepath, 
             const std::filesystem::path &texturePath = {});
    virtual ~ObjModel() override = default;

    void accept(ObjectVisitor &visitor) override;
    void update(float dt) override {};

    void loadFromOBJ(const std::filesystem::path &filepath);
    void loadTexture(const std::filesystem::path &texturePath);

    const Texture* getTexture() const noexcept { return texture.get(); }
    bool hasTexture() const noexcept { return texture != nullptr && texture->isLoaded(); }

    size_t getIndexCount() const noexcept { return indices.size(); }
    size_t getIndexDataSize() const noexcept { return indices.size() * sizeof(indices[0]); }
    const void* getRawIndexData() const { return indices.data(); }
    size_t getVertexDataSize() const noexcept { return vertices.size() * sizeof(vertices[0]); }
    const void* getRawVertexData() const { return vertices.data(); }
    size_t getVertexStride() const noexcept { return sizeof(Vertex); }

protected:
    std::filesystem::path filepath;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::shared_ptr<Texture> texture; 
};