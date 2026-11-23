#pragma once

#include "types.h"

// The counted reference fn_71003df480 returns (as src/game/CSpawnPointComponent/CSpawnPointComponent_c3_1.cpp).
struct UnkRefObj_c3 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // destroys the object
    s32 refs;
};
