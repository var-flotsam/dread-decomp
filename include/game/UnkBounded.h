#pragma once

#include "types.h"

struct UnkVec3f;

struct UnkBounded {
    virtual void vf0();
    virtual void vf8();
    virtual void vf10();
    virtual void vf18();
    virtual void vf20();
    virtual void vf28();
    virtual void vf30();
    virtual UnkVec3f GetMin();
    virtual UnkVec3f GetMax();
    u8 pad_0x8[0x18];
    s32 min_x10, min_y10, min_z10;
    s32 max_x10, max_y10, max_z10;
};
