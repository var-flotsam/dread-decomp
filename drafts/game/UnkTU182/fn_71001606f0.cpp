// Unit 182 of tools/tu_map.py (the graphics device layer): turns slot `index` of a slot group on or off.
// On a change the slot is marked dirty, its state value re-derived and the bind callback told of the
// resource made from the bound object (or of none). Names and layouts are placeholders.
#include "game/UnkGraphicsState.h"
#include "game/UnkTU172.h"
#include "types.h"

bool fn_710015bd80();
s32 fn_710015e9fc(s32* state);
extern void* (*g_unk_716e29ac90)(void* device, u32 a, u32 b);

struct UnkSlotBound_ovb0224 {
    void* ptr;
    s32 field_0x8;
    u8 pad_0xc[4];
    s32 state;  // 0x10
    u8 pad_0x14[0x5c];
    bool dirty;  // 0x70
    bool field_0x71;
    u8 pad_0x72[6];
};
static_assert(sizeof(UnkSlotBound_ovb0224) == sizeof(UnkGraphicsSlot), "slot size");

struct UnkBoundObject_ovb0224 {
    u8 pad_0x0[0xd8];
    u32 field_0xd8;
};

// Slot index of group group.
inline UnkSlotBound_ovb0224& Slot_l1606f0(u32 group, u32 index) {
    return reinterpret_cast<UnkSlotBound_ovb0224*>(g_unk_716e2a9730[group])[index];
}

// The parameters are (u32, bool, u32) as the code uses them; include/game/UnkTU182.h declares
// (s32, s32, bool), which the original's unsigned compare, bool mask and 32-bit group index rule out.
void fn_71001606f0(u32 index, bool enable, u32 group) {
    if (index > 15)
        return;
    if (!fn_710015bd80())
        return;
    if (Slot_l1606f0(group, index).field_0x71 == enable)
        return;
    Slot_l1606f0(group, index).field_0x71 = enable;
    UnkSlotBound_ovb0224& slot = Slot_l1606f0(group, index);
    slot.dirty = true;
    slot.field_0x8 = fn_710015e9fc(&slot.state);
    UnkBoundObject_ovb0224* object = static_cast<UnkBoundObject_ovb0224*>(slot.ptr);
    if (object && enable) {
        void* device = fn_7100153cd0();
        u32 id = object->field_0xd8;
        u32 mode = slot.field_0x8;
        void* resource = g_unk_716e29ac90(device, id, mode);
        void* context = fn_7100153d50();
        g_unk_716e29b858(context, 1, index, resource);
        return;
    }
    void* context = fn_7100153d50();
    g_unk_716e29b858(context, 1, index, nullptr);
}
