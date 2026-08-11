#include "shadowManager.h"
#include "shaders/shader.h"
#include "shadow/shadowMap/shadowMap.h"
#include <memory>

ShadowManager::ShadowManager(uint32_t resolution) {
    shadowMap = std::make_unique<ShadowMap>(resolution, resolution);
    shadowShader = std::make_shared<Shader>("src/shaders/shadow/shadowDepth.vs", "src/shaders/shadow/shadowDepth.fs");
}

void ShadowManager::beginShadowPass() {
    glViewport(0, 0, shadowMap->getWidth(), shadowMap->getHeight());
    shadowMap->bind();
    glClear(GL_DEPTH_BUFFER_BIT);

    glCullFace(GL_FRONT);
}

void ShadowManager::endShadowPass(int screenWidth, int screenHeight) {
    shadowMap->unbind();
    glCullFace(GL_BACK);
    glViewport(0, 0, screenWidth, screenHeight);
}

void ShadowManager::bindShadowMap(uint32_t slot) const {
    if (shadowMap) {
        shadowMap->bindTexture(slot);
    }
}
