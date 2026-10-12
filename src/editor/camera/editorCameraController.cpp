#include "editorCameraController.h"
#include <algorithm>

EditorCameraController::EditorCameraController(std::shared_ptr<Camera> camera)
    : camera(std::move(camera)) {}

void EditorCameraController::update(float dt, const InputState &input, bool viewportHovered) {
    if (!camera) {
        return;
    }
    if (!input.isDown(MouseButton::Right)) {
        looking = false;
        return;
    }
    if (!looking && !viewportHovered) {
        return;
    }
    looking = true;

    camera->processMouseMovement(input.mouseDelta.x, -input.mouseDelta.y);

    speed = std::clamp(speed * (1.0f + wheelSpeedModifier * input.wheel), minSpeed, maxSpeed);

    if (input.isDown(Key::W)) {
        camera->processKeyboard(MoveDirection::FORWARD, dt, speed);
    }
    if (input.isDown(Key::S)) {
        camera->processKeyboard(MoveDirection::BACKWARD, dt, speed);
    }
    if (input.isDown(Key::A)) {
        camera->processKeyboard(MoveDirection::LEFT, dt, speed);
    }
    if (input.isDown(Key::D)) {
        camera->processKeyboard(MoveDirection::RIGHT, dt, speed);
    }
}
