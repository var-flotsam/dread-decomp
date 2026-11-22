#pragma once

#include "game/UnkTU2744.h"
#include "types.h"

// The button as this function sees it.
struct UnkButton_ovb0108 {
    void** vtable;
    u8 pad_0x8[0x88];
    f32 step;  // 0x90
    u8 pad_0x94[0x9c];
    base::global::CStrId name;  // 0x130

    void vf_0x1d8(void* a) { reinterpret_cast<void (*)(UnkButton_ovb0108*, void*)>(vtable[0x1d8 / 8])(this, a); }
    void vf_0x78(s32 a, bool b) { reinterpret_cast<void (*)(UnkButton_ovb0108*, s32, bool)>(vtable[0x78 / 8])(this, a, b); }
};
