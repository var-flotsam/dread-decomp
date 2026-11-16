// Dropped-item manager's unit (2562): moves every active slot with its flag (bit 1) set that is farther
// than `step` from the target position by `step` towards it. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVectorOps.h"
#include "types.h"

using base::math::CVector3D;

struct UnkSlot_le2eef8 {
    u8 pad_0x0[0x75];
    u8 active;  // 0x75
    u8 pad_0x76[6];
    CVector3D pos;  // 0x7c
    u8 pad_0x88[0xa9 - 0x88];
    u8 flags;  // 0xa9
    u8 pad_0xaa[0xf0 - 0xaa];
};
static_assert(sizeof(UnkSlot_le2eef8) == 0xf0, "size");

struct UnkSlotManager_le2eef8 {
    u8 pad_0x0[8];
    UnkSlot_le2eef8 slots[128];  // 0x8
};

void fn_7100f06034(UnkSlot_le2eef8* slot, const CVector3D* pos);

void fn_7100e2eef8(UnkSlotManager_le2eef8* self, f32 step, const CVector3D* target) {
    for (UnkSlot_le2eef8& slot : self->slots) {
        if (!slot.active || !(slot.flags & 2))
            continue;
        CVector3D pos = slot.pos;
        CVector3D delta = pos - *target;
        if (LengthSq(delta) > step * step) {
            CVector3D moved = *target + Normalized(delta) * step;
            fn_7100f06034(&slot, &moved);
        }
    }
}
