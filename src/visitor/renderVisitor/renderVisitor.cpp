#include "renderVisitor.h"
#include "sceneObject/baseObject.h"
#include "sceneObject/primitive/lightSource/lightSource.h"
#include "sceneObject/primitive/model/model.h"
#include "sceneObject/primitive/model/texture/texture.h"
#include "sceneObject/primitive/particle/particle.h"
#include "sceneObject/primitive/particleEmitter/particleEmitter.h"
#include "sceneObject/primitive/camera/camera.h"
#include "shaders/shader.h"
#include <cstddef>
#include <glad/glad.h>
#include <memory>

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

OpenGLRenderVisitor::OpenGLRenderVisitor() {
    defaultShader = std::make_unique<Shader>("src/shaders/default/default.vs", "src/shaders/default/default.fs");
    particleShader = std::make_unique<Shader>("src/shaders/particle/particles.vs", "src/shaders/particle/particle.fs");
}

void OpenGLRenderVisitor::visit(Camera& camera) {
    curViewMatrix = camera.getViewMatrix();
    curProjMatrix = camera.getProjectionMatrix();
    curCameraPos = camera.getPosition();
}

void OpenGLRenderVisitor::visit(LightSource &source){
    curLightSource = source;
}

void OpenGLRenderVisitor::visit(Model& model) {
    const auto& handles = getOrUpload(model, [this](const Model& m) {
        return uploadToGPU(m);
    });

    defaultShader->use();
    // привязка текстуры
    const Texture* texture = model.getTexture();
    if (texture && texture->isLoaded()) {
        texture->bind(0);
        defaultShader->setInt("u_Texture", 0);
    }

    // параметры вершинного шейдера
    defaultShader->setMat4("u_View", curViewMatrix);
    defaultShader->setMat4("u_Projection", curProjMatrix);
    defaultShader->setMat4("u_Model", model.getModelMatrix());
    if (curLightSource.has_value()) {
        defaultShader->setMat4("u_LightSpaceMatrix", curLightSource->getLightSpaceMatrix());
    }

    // параметры фрагментного шейдера
    defaultShader->setVec3("u_ViewPos", curCameraPos);
    if (curLightSource.has_value()) {
        defaultShader->setVec3("u_LightPos", curLightSource->getPosition());
        defaultShader->setVec3("u_LightColor", curLightSource->getColor());
        defaultShader->setFloat("u_LightConstant", curLightSource->getAttenuationParams().constant);
        defaultShader->setFloat("u_LightLinear", curLightSource->getAttenuationParams().linear);
        defaultShader->setFloat("u_LightQuadratic", curLightSource->getAttenuationParams().quadratic);
    }

    glBindVertexArray(handles.getVAO());
    glDrawElements(GL_TRIANGLES, handles.getElementCount(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    if (texture && texture->isLoaded()) {
        texture->unbind();
    }

    cleanupExpiredCache();
}

void OpenGLRenderVisitor::visit(ParticleEmitter& emitter) {
    if (emitter.getParticleCount() == 0) {
        return;
    }

    const auto& handles = getOrUpload(emitter, [this](const ParticleEmitter& e) {
        return uploadToGPU(e);
    });

    particleShader->use();
    particleShader->setMat4("u_View", curViewMatrix);
    particleShader->setMat4("u_Projection", curProjMatrix);

    if (curLightSource.has_value()) {
        glm::vec3 worldLightDir = - curLightSource->getDirection();
        glm::vec3 viewLightDir = glm::vec3(curViewMatrix * glm::vec4(worldLightDir, 0.0));
        particleShader->setVec3("u_LightDir", viewLightDir);
    }
    particleShader->setVec4("u_WaterColor", glm::vec4(0.2f, 0.65f, 0.9f, 0.65f));
    particleShader->setFloat("u_MaxLife", 2.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    glBindBuffer(GL_ARRAY_BUFFER, handles.getInstanceVBO());
    size_t currentSizeBytes = emitter.getParticlesDataSizeBytes();
    glBufferData(GL_ARRAY_BUFFER, currentSizeBytes, emitter.getRawParticlesData(), GL_DYNAMIC_DRAW);

    glBindVertexArray(handles.getVAO());
    glDrawArraysInstanced(GL_TRIANGLES, 0, handles.getElementCount(), emitter.getParticleCount());
    
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    cleanupExpiredCache();
}

template<typename ObjectType, typename UploadFunc>
const GPUBufferHandles& OpenGLRenderVisitor::getOrUpload(ObjectType &obj, UploadFunc upload) {
    std::weak_ptr<const BaseObject> key = obj.shared_from_this();
    auto it = gpuCache.find(key);

    if (it == gpuCache.end()) {
        GPUBufferHandles handles = upload(obj);
        auto [insertedIt, success] = gpuCache.emplace(key, std::move(handles));
        it = insertedIt;
    }
    return it->second;
}

GPUBufferHandles OpenGLRenderVisitor::uploadToGPU(const Model &model) {
    GPUBufferHandles handles;
    handles.getElementCount() = model.getIndexCount();

    glGenVertexArrays(1, &handles.getVAO());
    glGenBuffers(1, &handles.getVBO());
    glGenBuffers(1, &handles.getEBO());
    glBindVertexArray(handles.getVAO());
    
    glBindBuffer(GL_ARRAY_BUFFER, handles.getVBO());
    glBufferData(GL_ARRAY_BUFFER, model.getVertexDataSize(), model.getRawVertexData(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handles.getEBO());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.getIndexDataSize(), model.getRawIndexData(), GL_STATIC_DRAW);

    const GLsizei stride = model.getVertexStride();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(Vertex::getPositionOffset()));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(Vertex::getNormalOffset()));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(Vertex::getTexCoordOffset()));

    glBindVertexArray(0);
    return handles;
}

GPUBufferHandles OpenGLRenderVisitor::uploadToGPU(const ParticleEmitter& emitter) {
    GPUBufferHandles handles;

    // 1. Статическая геометрия биллборда (квадрат 1x1 с центром в (0,0))
    // Каждая вершина: { Position.x, Position.y, TexCoord.u, TexCoord.v }
    static const float quadVertices[] = {
        -0.5f, -0.5f,   0.0f, 0.0f,
         0.5f, -0.5f,   1.0f, 0.0f,
        -0.5f,  0.5f,   0.0f, 1.0f,

        -0.5f,  0.5f,   0.0f, 1.0f,
         0.5f, -0.5f,   1.0f, 0.0f,
         0.5f,  0.5f,   1.0f, 1.0f
    };

    glGenVertexArrays(1, &handles.getVAO());
    glGenBuffers(1, &handles.getVBO());
    glGenBuffers(1, &handles.getInstanceVBO()); 

    glBindVertexArray(handles.getVAO());

    glBindBuffer(GL_ARRAY_BUFFER, handles.getVBO());
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    // Атрибут 0: Локальные координаты вершины квадрата (vec2)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));

    // Атрибут 1: Текстурные координаты (vec2)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, handles.getInstanceVBO());
    glBufferData(GL_ARRAY_BUFFER, emitter.getParticlesDataSizeBytes(), nullptr, GL_DYNAMIC_DRAW);

    const GLsizei stride = emitter.getParticleStride();
    // Атрибут 2: Позиция частицы в мировых координатах (glm::vec3)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(Particle::getPositionOffset()));
    glVertexAttribDivisor(2, 1);
    
    // Атрибут 3: Оставшееся время жизни частицы (float)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(Particle::getLifeOffset()));
    glVertexAttribDivisor(3, 1);

    // Атрибут 4: Радиус частицы (float)
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(Particle::getRadiusOffset()));
    glVertexAttribDivisor(4, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    handles.getElementCount() = 6;
    return handles;
}

void OpenGLRenderVisitor::cleanupExpiredCache() {
    for (auto it = gpuCache.begin(); it != gpuCache.end(); ) {
        if (it->first.expired()) {
            it = gpuCache.erase(it);
        } else {
            ++it;
        }
    }
}
