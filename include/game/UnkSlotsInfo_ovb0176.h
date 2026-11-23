#pragma once

// The slot table's state that unit 218 steps every frame (src/game/UnkTU218/UnkTU218_Slots_ovb0215.cpp) and the
// slot blend reads (src/game/UnkTU253/UnkTU253_SlotBlend_ovb0176.cpp). Names and layouts are placeholders.
#include "types.h"

struct UnkBoneArray;

struct UnkSlotObject_ovb0215 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(f32 dt);
};

struct UnkSlotsInfo_ovb0176 {
    u8 pad_0x0[0x30];
    u32 count;  // 0x30
    u8 pad_0x34[0x1c];
    UnkSlotObject_ovb0215** objects;  // 0x50
    u8 pad_0x58[0x28];
    UnkBoneArray* bones;  // 0x80
    u8 pad_0x88[0x48];
    u32 frame;  // 0xd0
};
static_assert(__builtin_offsetof(UnkSlotsInfo_ovb0176, frame) == 0xd0, "layout");
