#pragma once

#include "shadow/shadowMap/shadowMap.h"
#include "shaders/shader.h"
#include <memory>

class ShadowManager {
public:
    ShadowManager(uint32_t resolution = 2048);

    void beginShadowPass();
    void endShadowPass(int screenWidth, int screenHeight);

    void bindShadowMap(uint32_t slot = 1) const;

    const std::shared_ptr<Shader> getShadowShader() const noexcept { return shadowShader; }
    uint32_t getResolution() const noexcept { return shadowMap ? shadowMap->getWidth() : 0; }

protected:
    std::shared_ptr<Shader> shadowShader;
    std::unique_ptr<ShadowMap> shadowMap;
};
