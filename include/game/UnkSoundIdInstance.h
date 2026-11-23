#pragma once

#include "types.h"

// A playing instance: reference-counted (atomically, at +8), destroyed through its vtable.
struct UnkSoundIdInstance {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
    u8 pad_0xc[0x20];
    u32 state;  // 0x2c: 0xa and 0xc once it has finished
};
