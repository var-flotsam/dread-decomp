// Dropped-item manager's unit (2562): moves every active slot with its flag (bit 1) set that is farther
// than `step` from the target position by `step` towards it. Names and layouts are placeholders.
#include <math.h>

#include "types.h"

struct UnkVec3_le2eef8 {
    f32 x, y, z;
};

static inline UnkVec3_le2eef8 operator*(const UnkVec3_le2eef8& v, f32 s) {
    UnkVec3_le2eef8 r = {v.x * s, v.y * s, v.z * s};
    return r;
}
static inline UnkVec3_le2eef8 operator+(const UnkVec3_le2eef8& a, const UnkVec3_le2eef8& b) {
    UnkVec3_le2eef8 r = {a.x + b.x, a.y + b.y, a.z + b.z};
    return r;
}

struct UnkSlot_le2eef8 {
    u8 pad_0x0[0x75];
    u8 active;  // 0x75
    u8 pad_0x76[6];
    UnkVec3_le2eef8 pos;  // 0x7c
    u8 pad_0x88[0xa9 - 0x88];
    u8 flags;  // 0xa9
    u8 pad_0xaa[0xf0 - 0xaa];
};
static_assert(sizeof(UnkSlot_le2eef8) == 0xf0, "size");

struct UnkSlotManager_le2eef8 {
    u8 pad_0x0[8];
    UnkSlot_le2eef8 slots[128];  // 0x8
};

void fn_7100f06034(UnkSlot_le2eef8* slot, const UnkVec3_le2eef8* pos);

void fn_7100e2eef8(UnkSlotManager_le2eef8* self, f32 step, const UnkVec3_le2eef8* target) {
    for (UnkSlot_le2eef8& slot : self->slots) {
        if (!slot.active || !(slot.flags & 2))
            continue;
        f32 dx = slot.pos.x - target->x;
        f32 dy = slot.pos.y - target->y;
        f32 dz = slot.pos.z - target->z;
        f32 dist2 = dx * dx + dy * dy + dz * dz;
        if (dist2 > step * step) {
            f32 inv = 1.0f / (sqrtf(dist2) + 0.0001f);
            UnkVec3_le2eef8 delta = {dx, dy, dz};
            UnkVec3_le2eef8 moved = *target + delta * inv * step;
            fn_7100f06034(&slot, &moved);
        }
    }
}
