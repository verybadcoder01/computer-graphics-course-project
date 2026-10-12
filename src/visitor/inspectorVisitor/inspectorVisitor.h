#pragma once

#include <cstdint>
#include <functional>
#include "adapters/gui/guiAdapter.h"
#include "editor/core/editorCore.h"
#include "editor/ui/panels/propertyEditTracker.h"
#include "visitor/objectVisitor.h"

class BaseObject;

// draws the type-specific inspector sections; only the object with selectedId is drawn, which skips
// e.g. an emitter's nozzle mesh (ParticleEmitter::accept visits it too)
class InspectorVisitor : public ObjectVisitor {
   public:
    InspectorVisitor(IGuiAdapter &gui,
                     EditorCore &editor,
                     PropertyEditTracker &tracker,
                     uint32_t selectedId);

    void visit(Model &model) override;
    void visit(LightSource &light) override;
    void visit(Camera &camera) override;
    void visit(ParticleEmitter &emitter) override;

   private:
    std::reference_wrapper<IGuiAdapter> gui;
    std::reference_wrapper<EditorCore> editor;
    std::reference_wrapper<PropertyEditTracker> tracker;
    uint32_t selectedId;

    void drawTransform(BaseObject &object, bool withRotationAndScale);
    void drawCollider(Model &model);
    void applyColliderType(Model &model, int typeIndex);
};
