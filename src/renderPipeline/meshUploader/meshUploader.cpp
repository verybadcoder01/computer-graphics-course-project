// MeshUploader.cpp
#include "meshUploader.h"
#include <glad/glad.h>

GPUBufferHandles MeshUploader::uploadModelToGPU(const Model &model) {
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
    // vec3
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(Vertex::getPositionOffset()));

    glEnableVertexAttribArray(1);
    // vec3
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(Vertex::getNormalOffset()));

    glEnableVertexAttribArray(2);
    // vec2
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(Vertex::getTexCoordOffset()));

    glBindVertexArray(0);
    return handles;
}

GPUBufferHandles MeshUploader::uploadParticleEmitterToGPU(const ParticleEmitter& emitter) {
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
