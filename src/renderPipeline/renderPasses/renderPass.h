#pragma once

#include "renderPipeline/renderPasses/renderPipelineContext.h"
#include "scene/scene.h"

class IRenderPass {
   public:
    virtual ~IRenderPass() = default;

    virtual void execute(Scene &scene, RenderPipelineContext &ctx) = 0;

    void setEnabled(bool state) noexcept { enabled = state; }
    bool isEnabled() const noexcept { return enabled; }

   protected:
    bool enabled = true;
};