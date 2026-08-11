#include "sceneObject/baseObject.h"
#include "sceneObject/primitive/lightSource/lightSource.h"
#include "visitor/objectVisitor.h"
#include "sceneObject/primitive/model/model.h"
#include "shaders/shader.h"
#include "glad/glad.h"
#include <memory>
#include <map>
#include <optional>

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

class OpenGLRenderVisitor : public ObjectVisitor {
public:
    OpenGLRenderVisitor();
    ~OpenGLRenderVisitor() = default;

    void visit(Model &model) override;
    void visit(LightSource &light) override;
    void visit(Camera &camera) override;
    void visit(ParticleEmitter &emitter) override;

protected:
    std::map<std::weak_ptr<const BaseObject>, GPUBufferHandles, std::owner_less<std::weak_ptr<const BaseObject>>> gpuCache;
    
    std::unique_ptr<Shader> defaultShader;
    std::unique_ptr<Shader> particleShader;
    glm::mat4 curViewMatrix{1};
    glm::mat4 curProjMatrix{1};
    glm::vec3 curCameraPos{0};
    std::optional<LightSource> curLightSource;

    template<typename ObjectType, typename UploadFunc>
    const GPUBufferHandles& getOrUpload(ObjectType &obj, UploadFunc upload);

    GPUBufferHandles uploadToGPU(const Model &model);
    GPUBufferHandles uploadToGPU(const ParticleEmitter &emitter);
    void cleanupExpiredCache();
};
