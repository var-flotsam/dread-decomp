// Dropped-item manager's unit (2562): for every active slot with its flag (bit 1) set, hands the owner
// to the slot under the name "DC_RootMotion". Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkPickable_ovb0083;
void fn_7100f05a5c(UnkPickable_ovb0083* self, UnkWeakTarget* owner, const CStrId& name);
void* fn_7100856b98(void* owner);

struct UnkSlot_le2f030 {
    u8 pad_0x0[0x75];
    u8 active;  // 0x75
    u8 pad_0x76[0x33];
    u8 flags;  // 0xa9
    u8 pad_0xaa[0xf0 - 0xaa];
};
static_assert(sizeof(UnkSlot_le2f030) == 0xf0, "size");

struct UnkSlotManager_le2f030 {
    u8 pad_0x0[8];
    UnkSlot_le2f030 slots[128];  // 0x8
};
static_assert(sizeof(UnkSlotManager_le2f030) == 0x7808, "size");

void fn_7100e2f030(UnkSlotManager_le2f030* self, UnkWeakTarget* owner) {
    if (!owner)
        return;
    if (!fn_7100856b98(owner))
        return;
    CStrId name("DC_RootMotion", true);
    for (UnkSlot_le2f030& slot : self->slots)
        if (slot.active && (slot.flags & 2))
            fn_7100f05a5c(reinterpret_cast<UnkPickable_ovb0083*>(&slot), owner, name);
}
