#include <functional>
#include "glad/glad.h"
#include "renderPipeline/gpuCache/gpuCacheManager.h"
#include "renderPipeline/renderContext.h"
#include "sceneObject/primitive/lightSource/lightSource.h"
#include "sceneObject/primitive/model/model.h"
#include "shaders/shader.h"
#include "visitor/objectVisitor.h"

class OpenGLRenderVisitor : public ObjectVisitor {
   public:
    OpenGLRenderVisitor(const RenderContext &context,
                        GPUCacheManager &cacheManager,
                        const Shader &defaultShader,
                        const Shader &particleShader);
    ~OpenGLRenderVisitor() = default;

    void visit(Model &model) override;
    void visit(LightSource &light) override {};
    void visit(Camera &camera) override {};
    void visit(ParticleEmitter &emitter) override;

   protected:
    std::reference_wrapper<const Shader> defaultShader;
    std::reference_wrapper<const Shader> particleShader;
    std::reference_wrapper<const RenderContext> ctx;
    std::reference_wrapper<GPUCacheManager> cache;
};
