#pragma once

#include "game/UnkSphere.h"

// A box with virtual accessors (0x20 bytes): its minimum and maximum corners. The same class as
// src/game/UnkSpherePool.cpp's (its base constructor and vtable are shared).
struct UnkPoolBox {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void SetMin(const UnkVec3f* v);  // vf_0x28
    virtual void SetMax(const UnkVec3f* v);  // vf_0x30
    virtual UnkVec3f GetMin();               // vf_0x38
    virtual UnkVec3f GetMax();               // vf_0x40
    UnkVec3f min;
    UnkVec3f max;
};
