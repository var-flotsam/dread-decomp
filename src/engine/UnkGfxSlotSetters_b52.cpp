// Unit 182 of tools/tu_map.py: two more setters of one float of a per-group graphics slot, copies of
// fn_7100160c30 (src/engine/UnkGfxSlotSetters_b50.cpp) at offsets 0x38 and 0x3c. The signatures are the
// ones src/engine/UnkGraphicsSystem.cpp calls them with (the third argument is the group). Names are
// placeholders.
#include "game/UnkGraphicsState.h"
#include "types.h"

template <u32 Offset>
static inline f32& SlotFloat(UnkGraphicsSlot& s) {
    return *reinterpret_cast<f32*>(reinterpret_cast<u8*>(&s) + Offset);
}

void fn_7100160bd0(u32 a, f32 value, u32 c) {
    if (a >= 16)
        return;
    if (SlotFloat<0x38>(g_unk_716e2a9730[c][a]) != value) {
        SlotFloat<0x38>(g_unk_716e2a9730[c][a]) = value;
        g_unk_716e2a9730[c][a].dirty = true;
    }
}

void fn_7100160c90(u32 a, f32 value, u32 c) {
    if (a >= 16)
        return;
    if (SlotFloat<0x3c>(g_unk_716e2a9730[c][a]) != value) {
        SlotFloat<0x3c>(g_unk_716e2a9730[c][a]) = value;
        g_unk_716e2a9730[c][a].dirty = true;
    }
}
