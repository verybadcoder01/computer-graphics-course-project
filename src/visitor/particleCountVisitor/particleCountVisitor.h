#pragma once

#include <cstddef>
#include "visitor/objectVisitor.h"

class ParticleCountVisitor : public ObjectVisitor {
   public:
    void visit(Model &) override {}
    void visit(LightSource &) override {}
    void visit(Camera &) override {}
    void visit(ParticleEmitter &emitter) override;

    size_t getCount() const noexcept { return count; }

   private:
    size_t count = 0;
};
