#pragma once

#include <filesystem>
#include <memory>
#include <vector>
#include "sceneObject/collider/baseCollider.h"
#include "sceneObject/primitive/model/model.h"
#include "sceneObject/primitive/model/texture/texture.h"
#include "visitor/objectVisitor.h"

class ObjModel : public Model {
   public:
    ObjModel() = default;
    ObjModel(const std::string &name,
             const std::filesystem::path &filepath,
             const std::filesystem::path &texturePath = {});
    virtual ~ObjModel() override = default;

    void accept(ObjectVisitor &visitor) override;
    void update(const SimulationContext &ctx) override {};

    void loadFromOBJ(const std::filesystem::path &filepath);
    void loadTexture(const std::filesystem::path &texturePath);

    void setCollider(std::shared_ptr<Collider> coll) noexcept override { collider = coll; };
    std::shared_ptr<Collider> getCollider() const override { return collider; };

    const Texture *getTexture() const noexcept override { return texture.get(); }
    bool hasTexture() const noexcept { return texture != nullptr && texture->isLoaded(); }

    const std::vector<Vertex> &getVertices() const noexcept override { return vertices; }

    size_t getIndexCount() const noexcept override { return indices.size(); }
    size_t getIndexDataSize() const noexcept override {
        return indices.size() * sizeof(indices[0]);
    }
    const void *getRawIndexData() const override { return indices.data(); }
    size_t getVertexDataSize() const noexcept override {
        return vertices.size() * sizeof(vertices[0]);
    }
    const void *getRawVertexData() const override { return vertices.data(); }
    size_t getVertexStride() const noexcept override { return sizeof(Vertex); }

   protected:
    std::filesystem::path filepath;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::shared_ptr<Texture> texture;
    std::shared_ptr<Collider> collider;
};