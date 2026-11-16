#pragma once

// The E.M.M.I. AI component as the CEmmyAIComponent unit's state functions see it (tools/tu_map.py unit 811):
// fn_71005590fc and fn_710054095c. Only the members those touch are spelled out. Names and layout are
// placeholders.
#include "types.h"

struct UnkEmmyOwner2_b57a3 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkEmmyAI2_b57a3 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkEmmyOwner2_b57a3* owner;  // 0x30
    u8 pad_0x38[0x14c35 - 0x38];
    u8 barrierFlag;  // 0x14c35: the barrier state is reported (fn_710054095c)
    u8 pad_0x14c36[0x27720 - 0x14c36];
    bool field_0x27720;  // set: the barrier is up (fn_710054095c reports the barrier name)
    u8 pad_0x27721[0x29eec - 0x27721];
    u32 field_0x29eec;
    s32 field_0x29ef0;

    void vf_0x850() { reinterpret_cast<void (*)(UnkEmmyAI2_b57a3*)>(vtable[0x850 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkEmmyAI2_b57a3, field_0x29ef0) == 0x29ef0, "UnkEmmyAI2_b57a3 layout");
