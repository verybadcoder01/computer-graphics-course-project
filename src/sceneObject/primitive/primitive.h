#pragma once

#include "sceneObject/baseObject.h"

typedef unsigned int VideoMemoryDesc;

class Primitive : public BaseObject {
   public:
    virtual ~Primitive() override = default;
};