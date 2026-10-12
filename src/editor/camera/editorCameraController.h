#pragma once

#include <memory>
#include "adapters/gui/guiAdapter.h"
#include "sceneObject/primitive/camera/camera.h"

// fly camera: hold the right mouse button over the viewport to look around, WASD to move,
// mouse wheel to change the speed
class EditorCameraController {
   public:
    explicit EditorCameraController(std::shared_ptr<Camera> camera);

    void update(float dt, const InputState &input, bool viewportHovered);

   private:
    std::shared_ptr<Camera> camera;
    float speed = 5.0f;
    bool looking = false;
    static constexpr float wheelSpeedModifier = 0.1f;
    static constexpr float minSpeed = 0.5f, maxSpeed = 100.0f;
};
