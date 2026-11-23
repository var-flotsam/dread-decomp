#pragma once

#include "game/UnkXformNode.h"

struct UnkRenderer_b260 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void Draw(void* list, u64 a, void* b, f32 t, u32 c, u32 d, UnkVec3f* v);  // vf_0x20
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void SetHandles(void** handles, u32 count);  // vf_0x38
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void Finish();  // vf_0x50
};
