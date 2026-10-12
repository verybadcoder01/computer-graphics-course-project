#pragma once

#include "sceneObject/primitive/model/model.h"
#include "sceneObject/primitive/particle/particle.h"
#include "sceneObject/primitive/primitive.h"
#include "visitor/objectVisitor.h"

class ParticleEmitter : public Primitive {
   public:
    ParticleEmitter(size_t particleCount,
                    const ParticleSystemParams &params,
                    std::shared_ptr<Model> visualMesh);
    virtual ~ParticleEmitter() = default;

    void accept(ObjectVisitor &visitor) override;
    void update(const SimulationContext &ctx) override;
    bool isVisible() const override { return visualMesh ? true : false; }

    const ParticleSystemParams &getParticlesParams() const noexcept {
        return particles->getParams();
    };
    void setParticlesParams(const ParticleSystemParams &params) noexcept {
        particles->setParams(params);
    }
    void setParticleCount(size_t count) { particles->setParticleCount(count); }
    void resetParticles() noexcept { particles->killAll(); }

    size_t getParticleCount() const noexcept { return particles->getParticles().size(); }
    size_t getParticlesDataSizeBytes() const noexcept {
        return particles->getParticlesDataSizeBytes();
    }
    const void *getRawParticlesData() const { return particles->getRawParticlesData(); }
    size_t getParticleStride() const { return particles->getParticleStride(); }

   protected:
    std::unique_ptr<ParticleSystem> particles = nullptr;
    std::shared_ptr<Model> visualMesh = nullptr;

    void linkVisualMesh();
};
