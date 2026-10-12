#include "inspectorVisitor.h"
#include <cmath>
#include <glm/trigonometric.hpp>
#include <string>
#include <vector>
#include "editor/command/commands/setColliderCommand.h"
#include "exceptions/baseException.h"
#include "sceneObject/collider/boxCollider.h"
#include "sceneObject/collider/hemisphereCollider.h"
#include "sceneObject/collider/sphereCollider.h"
#include "sceneObject/primitive/camera/camera.h"
#include "sceneObject/primitive/lightSource/lightSource.h"
#include "sceneObject/primitive/model/model.h"
#include "sceneObject/primitive/particleEmitter/particleEmitter.h"

namespace {

constexpr float defaultBounce = 0.4f;
constexpr int maxParticleCount = 200000;

template <typename Derived>
std::shared_ptr<Derived> sharedFrom(BaseObject &object) {
    return std::static_pointer_cast<Derived>(object.shared_from_this());
}

}  // namespace

InspectorVisitor::InspectorVisitor(IGuiAdapter &gui,
                                   EditorCore &editor,
                                   PropertyEditTracker &tracker,
                                   uint32_t selectedId)
    : gui(gui), editor(editor), tracker(tracker), selectedId(selectedId) {}

void InspectorVisitor::visit(Model &model) {
    if (model.getId() != selectedId) {
        return;
    }
    drawTransform(model, true);
    drawCollider(model);
}

void InspectorVisitor::visit(ParticleEmitter &emitter) {
    if (emitter.getId() != selectedId) {
        return;
    }
    drawTransform(emitter, true);

    IGuiAdapter &g = gui.get();
    if (!g.collapsingHeader("Emitter")) {
        return;
    }
    auto self = sharedFrom<ParticleEmitter>(emitter);

    int count = static_cast<int>(emitter.getParticleCount());
    const int countBefore = count;
    if (g.dragInt("Particles", count, 10.0f, 0, maxParticleCount)) {
        emitter.setParticleCount(static_cast<size_t>(count));
    }
    tracker.get().track<int>(g, editor, countBefore, count, [self](const int &v) {
        self->setParticleCount(static_cast<size_t>(v));
    });

    const auto setParams = [self](const ParticleSystemParams &p) { self->setParticlesParams(p); };
    ParticleSystemParams params = emitter.getParticlesParams();
    const ParticleSystemParams paramsBefore = params;

    glm::vec3 startVelocity{params.startVx, params.startVy, params.startVz};
    if (g.dragFloat3("Start velocity", startVelocity, 0.05f)) {
        params.startVx = startVelocity.x;
        params.startVy = startVelocity.y;
        params.startVz = startVelocity.z;
        emitter.setParticlesParams(params);
    }
    tracker.get().track<ParticleSystemParams>(g, editor, paramsBefore, params, setParams);

    const auto rangeField = [&](const std::string &label, float &value, float lo, float hi) {
        const ParticleSystemParams before = params;
        if (g.dragFloat(label, value, 0.02f, lo, hi)) {
            emitter.setParticlesParams(params);
        }
        tracker.get().track<ParticleSystemParams>(g, editor, before, params, setParams);
    };
    rangeField("Velocity spread min", params.minVel, -100.0f, params.maxVel);
    rangeField("Velocity spread max", params.maxVel, params.minVel, 100.0f);
    rangeField("Life min", params.minLife, 0.01f, params.maxLife);
    rangeField("Life max", params.maxLife, params.minLife, 60.0f);
}

void InspectorVisitor::visit(LightSource &light) {
    if (light.getId() != selectedId) {
        return;
    }
    drawTransform(light, false);

    IGuiAdapter &g = gui.get();
    if (!g.collapsingHeader("Light")) {
        return;
    }
    auto self = sharedFrom<LightSource>(light);

    const std::vector<std::string> types = {"Directional", "Point", "Spot"};
    const int typeBefore = static_cast<int>(light.getType());
    int type = typeBefore;
    if (g.combo("Type", type, types) && type != typeBefore) {
        PropertyEditTracker::commit<int>(editor, typeBefore, type, [self](const int &v) {
            self->setType(static_cast<LightType>(v));
        });
    }

    // direction is edited as two angles; editing raw vector components fights normalization
    const glm::vec3 dirBefore = light.getDirection();
    float elevation = glm::degrees(std::asin(glm::clamp(dirBefore.y, -1.0f, 1.0f)));
    float azimuth = glm::degrees(std::atan2(dirBefore.z, dirBefore.x));
    const auto toDirection = [](float azimuthDeg, float elevationDeg) {
        const float az = glm::radians(azimuthDeg), el = glm::radians(elevationDeg);
        return glm::vec3(std::cos(el) * std::cos(az), std::sin(el), std::cos(el) * std::sin(az));
    };
    const auto setDirection = [self](const glm::vec3 &v) { self->setDirection(v); };

    if (g.dragFloat("Azimuth", azimuth, 0.5f, -180.0f, 180.0f)) {
        light.setDirection(toDirection(azimuth, elevation));
    }
    tracker.get().track<glm::vec3>(g, editor, dirBefore, light.getDirection(), setDirection);
    if (g.dragFloat("Elevation", elevation, 0.5f, -90.0f, 90.0f)) {
        light.setDirection(toDirection(azimuth, elevation));
    }
    tracker.get().track<glm::vec3>(g, editor, dirBefore, light.getDirection(), setDirection);

    glm::vec3 color = light.getBaseColor();
    const glm::vec3 colorBefore = color;
    if (g.colorEdit3("Color", color)) {
        light.setColor(color);
    }
    tracker.get().track<glm::vec3>(g, editor, colorBefore, color,
                                   [self](const glm::vec3 &v) { self->setColor(v); });

    float intensity = light.getIntensity();
    const float intensityBefore = intensity;
    if (g.dragFloat("Intensity", intensity, 0.01f, 0.0f, 100.0f)) {
        light.setIntensity(intensity);
    }
    tracker.get().track<float>(g, editor, intensityBefore, intensity,
                               [self](const float &v) { self->setIntensity(v); });

    bool shadows = light.createsShadows();
    if (g.checkbox("Casts shadows", shadows)) {
        PropertyEditTracker::commit<bool>(editor, !shadows, shadows,
                                          [self](const bool &v) { self->setCastsShadows(v); });
    }
}

void InspectorVisitor::visit(Camera &camera) {
    if (camera.getId() != selectedId) {
        return;
    }
    drawTransform(camera, false);

    IGuiAdapter &g = gui.get();
    if (!g.collapsingHeader("Camera")) {
        return;
    }
    auto self = sharedFrom<Camera>(camera);
    float fov = camera.getFov();
    const float fovBefore = fov;
    if (g.dragFloat("FOV", fov, 0.2f, 10.0f, 120.0f)) {
        camera.setFov(fov);
    }
    tracker.get().track<float>(g, editor, fovBefore, fov,
                               [self](const float &v) { self->setFov(v); });
}

void InspectorVisitor::drawTransform(BaseObject &object, bool withRotationAndScale) {
    IGuiAdapter &g = gui.get();
    if (!g.collapsingHeader("Transform")) {
        return;
    }
    auto self = object.shared_from_this();

    glm::vec3 position = glm::vec3(object.getPosition());
    const glm::vec3 positionBefore = position;
    if (g.dragFloat3("Position", position, 0.05f)) {
        object.setPosition(position);
    }
    tracker.get().track<glm::vec3>(g, editor, positionBefore, position,
                                   [self](const glm::vec3 &v) { self->setPosition(v); });

    if (!withRotationAndScale) {
        return;
    }

    glm::vec3 rotation = object.getRotation();
    const glm::vec3 rotationBefore = rotation;
    if (g.dragFloat3("Rotation", rotation, 0.5f)) {
        object.setRotation(rotation);
    }
    tracker.get().track<glm::vec3>(g, editor, rotationBefore, rotation,
                                   [self](const glm::vec3 &v) { self->setRotation(v); });

    glm::vec3 scale = object.getScale();
    const glm::vec3 scaleBefore = scale;
    if (g.dragFloat3("Scale", scale, 0.01f)) {
        object.setScale(scale);
    }
    tracker.get().track<glm::vec3>(g, editor, scaleBefore, scale,
                                   [self](const glm::vec3 &v) { self->setScale(v); });
}

void InspectorVisitor::drawCollider(Model &model) {
    IGuiAdapter &g = gui.get();
    if (!g.collapsingHeader("Collider")) {
        return;
    }

    const auto collider = model.getCollider();
    const std::vector<std::string> types = {"None", "Sphere", "Box", "Bowl"};
    const int typeBefore = collider ? static_cast<int>(collider->getType()) + 1 : 0;
    int type = typeBefore;
    if (g.combo("Type##collider", type, types) && type != typeBefore) {
        applyColliderType(model, type);
        return;
    }
    if (!collider) {
        return;
    }

    float bounce = collider->getBounceCoeff();
    const float bounceBefore = bounce;
    if (g.dragFloat("Bounce", bounce, 0.01f, 0.0f, 1.0f)) {
        collider->setBounceCoeff(bounce);
    }
    tracker.get().track<float>(g, editor, bounceBefore, bounce,
                               [collider](const float &v) { collider->setBounceCoeff(v); });
}

void InspectorVisitor::applyColliderType(Model &model, int typeIndex) {
    const float bounce =
        model.getCollider() ? model.getCollider()->getBounceCoeff() : defaultBounce;
    std::shared_ptr<Collider> newCollider = nullptr;
    try {
        switch (typeIndex) {
            case 1:
                newCollider = std::make_shared<SphereCollider>(model, bounce);
                break;
            case 2:
                newCollider = std::make_shared<BoxCollider>(model, bounce);
                break;
            case 3:
                newCollider = std::make_shared<ThickHemisphereCollider>(model, bounce);
                break;
            default:
                break;
        }
    } catch (const BaseException &e) {
        editor.get().setStatusMessage(e.what());
        return;
    }
    editor.get().executeCommand(
        std::make_unique<SetColliderCommand>(sharedFrom<Model>(model), std::move(newCollider)));
}
