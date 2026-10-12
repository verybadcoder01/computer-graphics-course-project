#include "viewport.h"
#include <algorithm>

Viewport::Viewport(std::shared_ptr<BasePipeline> pipeline,
                   std::shared_ptr<ObjectPickingPass> pickingPass,
                   std::shared_ptr<Camera> camera)
    : pipeline(std::move(pipeline)),
      pickingPass(std::move(pickingPass)),
      camera(std::move(camera)),
      framebuffer(1, 1) {
    if (this->pickingPass) {
        this->pickingPass->setEnabled(false);
    }
}

void Viewport::resize(int width, int height) {
    width = std::max(width, 1);
    height = std::max(height, 1);
    if (width == framebuffer.getWidth() && height == framebuffer.getHeight()) {
        return;
    }
    framebuffer.resize(width, height);
    if (camera) {
        camera->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
    }
}

void Viewport::requestPick(int x, int y) noexcept {
    pendingPick = glm::ivec2(x, y);
}

std::optional<uint32_t> Viewport::render(Scene &scene) {
    const bool picking = pendingPick.has_value() && pickingPass;
    if (picking) {
        pickingPass->setEnabled(true);
    }

    pipeline->execute(scene, framebuffer.getWidth(), framebuffer.getHeight(), framebuffer.getFBO());

    if (!picking) {
        pendingPick.reset();
        return std::nullopt;
    }
    uint32_t id = pickingPass->readObjectAt(pendingPick->x, pendingPick->y);
    pickingPass->setEnabled(false);
    pendingPick.reset();
    return id;
}
