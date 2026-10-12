#pragma once

#include "visitor/objectVisitor.h"

class ParticleResetVisitor : public ObjectVisitor {
   public:
    void visit(Model &) override {}
    void visit(LightSource &) override {}
    void visit(Camera &) override {}
    void visit(ParticleEmitter &emitter) override;
};
