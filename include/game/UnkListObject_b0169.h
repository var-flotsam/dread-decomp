#pragma once

#include "game/UnkReadyObject_b0169.h"

// A list-like object: ready, size and element accessors.
struct UnkListObject_b0169 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual bool vf_0x20();
    virtual s32 vf_0x28();
    virtual UnkReadyObject_b0169* vf_0x30(s32 index);
};
