#include "renderVisitor.h"
#include <glad/glad.h>
#include "renderPipeline/meshUploader/meshUploader.h"
#include "renderPipeline/renderContext.h"
#include "sceneObject/primitive/model/model.h"
#include "sceneObject/primitive/model/texture/texture.h"
#include "sceneObject/primitive/particleEmitter/particleEmitter.h"
#include "shaders/shader.h"

OpenGLRenderVisitor::OpenGLRenderVisitor(const RenderContext &context,
                                         GPUCacheManager &cacheManager,
                                         const Shader &defaultShader,
                                         const Shader &particleShader,
                                         const Texture &fallbackTexture)
    : defaultShader(defaultShader),
      particleShader(particleShader),
      ctx(context),
      cache(cacheManager),
      fallbackTexture(fallbackTexture) {}

void OpenGLRenderVisitor::visit(Model &model) {
    if (!model.isVisible()) {
        return;
    }
    const auto &handles = cache.get().getOrUpload<Model>(model, MeshUploader::uploadModelToGPU);

    const Shader &defShader = defaultShader.get();
    const RenderContext &rCtx = ctx.get();
    defShader.use();
    defShader.setMat4("u_View", rCtx.viewMatrix);
    defShader.setMat4("u_Projection", rCtx.projMatrix);
    defShader.setMat4("u_Model", model.getWorldMatrix());
    defShader.setVec3("u_ViewPos", rCtx.cameraPos);

    if (rCtx.mainLight) {
        defShader.setMat4("u_LightSpaceMatrix", rCtx.mainLight->getLightSpaceMatrix());
        defShader.setInt("u_LightType", static_cast<int>(rCtx.mainLight->getType()));
        defShader.setVec3("u_LightDir", rCtx.mainLight->getDirection());
        defShader.setVec3("u_LightPos", rCtx.mainLight->getPosition());
        defShader.setVec3("u_LightColor", rCtx.mainLight->getColor());

        const auto &att = rCtx.mainLight->getAttenuationParams();
        defShader.setFloat("u_LightConstant", att.constant);
        defShader.setFloat("u_LightLinear", att.linear);
        defShader.setFloat("u_LightQuadratic", att.quadratic);
    }

    if (const auto *tex = model.getTexture(); tex && tex->isLoaded()) {
        tex->bind(0);
    } else {
        fallbackTexture.get().bind(0);
    }
    defShader.setInt("u_Texture", 0);

    if (rCtx.shadowMapTextureId != 0) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, rCtx.shadowMapTextureId);
    } else {
        fallbackTexture.get().bind(1);
    }
    defShader.setInt("u_ShadowMap", 1);

    glBindVertexArray(handles.getVAO());
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(handles.getElementCount()), GL_UNSIGNED_INT,
                   nullptr);
    glBindVertexArray(0);
}

void OpenGLRenderVisitor::visit(ParticleEmitter &emitter) {
    if (emitter.getParticleCount() == 0) {
        return;
    }

    const auto &handles =
        cache.get().getOrUpload<ParticleEmitter>(emitter, MeshUploader::uploadParticleEmitterToGPU);

    const Shader &pShader = particleShader.get();
    const RenderContext &rCtx = ctx.get();
    pShader.use();
    pShader.setMat4("u_View", rCtx.viewMatrix);
    pShader.setMat4("u_Projection", rCtx.projMatrix);

    if (rCtx.mainLight) {
        glm::vec3 worldLightDir = -rCtx.mainLight->getDirection();
        glm::vec3 viewLightDir = glm::vec3(rCtx.viewMatrix * glm::vec4(worldLightDir, 0.0f));
        pShader.setVec3("u_LightDir", viewLightDir);
    }
    pShader.setVec4("u_WaterColor", glm::vec4(0.2f, 0.65f, 0.9f, 0.65f));
    pShader.setFloat("u_MaxLife", emitter.getParticlesParams().maxLife);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    glBindBuffer(GL_ARRAY_BUFFER, handles.getInstanceVBO());
    glBufferData(GL_ARRAY_BUFFER, emitter.getParticlesDataSizeBytes(),
                 emitter.getRawParticlesData(), GL_DYNAMIC_DRAW);

    glBindVertexArray(handles.getVAO());
    glDrawArraysInstanced(GL_TRIANGLES, 0, handles.getElementCount(), emitter.getParticleCount());

    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}
