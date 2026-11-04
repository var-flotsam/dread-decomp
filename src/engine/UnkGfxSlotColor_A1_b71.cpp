// Unit 182 of tools/tu_map.py: the colour setter of a per-group graphics slot (the setters of
// src/engine/UnkGfxSlotSetters_b52.cpp): a packed RGBA colour at +0x34, unpacked into four floats
// (0-1, red from the top byte) at +0x54 when it changes. The third argument is the group. Names are
// placeholders.
#include "game/UnkGraphicsState.h"
#include "types.h"

template <u32 Offset, class T>
static inline T& SlotField(UnkGraphicsSlot& s) {
    return *reinterpret_cast<T*>(reinterpret_cast<u8*>(&s) + Offset);
}

void fn_7100160b30(u32 a, u32 color, u32 c) {
    if (a >= 16)
        return;
    if (SlotField<0x34, u32>(g_unk_716e2a9730[c][a]) != color) {
        SlotField<0x34, u32>(g_unk_716e2a9730[c][a]) = color;
        SlotField<0x54, f32>(g_unk_716e2a9730[c][a]) = (f32)(color >> 24) / 255.0f;
        SlotField<0x58, f32>(g_unk_716e2a9730[c][a]) = (f32)((color >> 16) & 0xff) / 255.0f;
        SlotField<0x5c, f32>(g_unk_716e2a9730[c][a]) = (f32)((color >> 8) & 0xff) / 255.0f;
        SlotField<0x60, f32>(g_unk_716e2a9730[c][a]) = (f32)(color & 0xff) / 255.0f;
        g_unk_716e2a9730[c][a].dirty = true;
    }
}
