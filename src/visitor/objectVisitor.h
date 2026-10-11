#pragma once

class Model;
class LightSource;
class Camera;
class ParticleEmitter;

class ObjectVisitor {
   public:
    virtual ~ObjectVisitor() = default;

    virtual void visit(Model &model) = 0;
    virtual void visit(LightSource &light) = 0;
    virtual void visit(Camera &camera) = 0;
    virtual void visit(ParticleEmitter &emitter) = 0;
};