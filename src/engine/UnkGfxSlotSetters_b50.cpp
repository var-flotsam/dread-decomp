// Unit 182 of tools/tu_map.py, beside fn_7100160990: setters of one value of a per-group graphics slot
// (include/game/UnkGraphicsState.h) that flag the slot dirty on a change. Names are placeholders.
#include "game/UnkGraphicsState.h"
#include "types.h"

static inline f32& SlotValue0x44(UnkGraphicsSlot& s) {
    return *reinterpret_cast<f32*>(reinterpret_cast<u8*>(&s) + 0x44);
}
static inline u32& SlotValue0x30(UnkGraphicsSlot& s) {
    return *reinterpret_cast<u32*>(reinterpret_cast<u8*>(&s) + 0x30);
}

void fn_7100160c30(u32 index, u32 slot, f32 value) {
    if (index >= 16)
        return;
    if (SlotValue0x44(g_unk_716e2a9730[slot][index]) != value) {
        SlotValue0x44(g_unk_716e2a9730[slot][index]) = value;
        g_unk_716e2a9730[slot][index].dirty = true;
    }
}

void fn_7100160cf0(u32 index, u32 value, u32 slot) {
    if (index >= 16)
        return;
    if (SlotValue0x30(g_unk_716e2a9730[slot][index]) == value)
        return;
    SlotValue0x30(g_unk_716e2a9730[slot][index]) = value;
    g_unk_716e2a9730[slot][index].dirty = true;
}
