#pragma once

#include "types.h"

struct UnkShelmitObject_b56a3 {
    virtual void vf_0x0();
    u8 pad_0x8[0x48];
    f32 field_0x50;
    f32 field_0x54;

    void vf_0x168(f32 value, bool flag) {
        reinterpret_cast<void (*)(UnkShelmitObject_b56a3*, f32, bool)>((*reinterpret_cast<void***>(this))[0x168 / 8])(this, value, flag);
    }
    void vf_0x170(f32 value) {
        reinterpret_cast<void (*)(UnkShelmitObject_b56a3*, f32)>((*reinterpret_cast<void***>(this))[0x170 / 8])(this, value);
    }
};
