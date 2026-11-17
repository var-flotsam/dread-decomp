// The CDropComponent unit (tools/tu_map.py unit 1223): picks a random speed and direction for one
// drop. The direction goes into the least used of eight sectors (counts in `slots`, -1 for a closed
// one), with a random spread and height, and is normalized. Names and layouts are placeholders.
#include <math.h>
#include <stdlib.h>

#include "base/math/CVector.h"
#include "game/CCoreXAIComponent.h"
#include "types.h"

using base::math::CVector3D;

struct UnkDropComp3_ovb0096;

struct UnkDropScatterParams_l7c4a54 {
    u8 pad_0x0[0x50];
    f32 min_speed;
    f32 max_speed;
    f32 max_height;
};

struct UnkDropOwnerData_l7c4a54 {
    u8 pad_0x0[0x160];
    f32 extra_speed;
};

// The angle between two sectors.
extern f32 g_unk_7101a715b8;

static inline f32 RandomUnit() { return (rand() & 0x7fff) * (1.0 / 32767.0); }

// A random value between min and max.
static inline f32 RandomRange(f32 min, f32 max) {
    f32 t = RandomUnit();
    return min + (max - min) * t;
}

static inline u32 NextSlot(u32 slot) { return slot + 1 > 7 ? 0 : slot + 1; }

void fn_71007c4a54(UnkDropComp3_ovb0096* self, void* params, CVector3D* dir, f32* speed, u32* slots) {
    auto* p = static_cast<UnkDropScatterParams_l7c4a54*>(params);
    auto* owner = reinterpret_cast<UnkDropOwnerData_l7c4a54*>(fn_71004dad10(reinterpret_cast<UnkActor*>(self)));
    f32 extra = owner ? owner->extra_speed : 0.0f;
    *speed = extra + RandomRange(p->min_speed, p->max_speed);

    // Start at a random sector and skip the closed ones.
    u32 slot = rand() & 7;
    u32 count = slots[slot];
    if (count == 0xffffffff) {
        u32 start = slot;
        do {
            slot = NextSlot(slot);
            count = slots[slot];
        } while (count == 0xffffffff && slot != start);
    }
    // Then take the first open sector further on that has been used less.
    if (count != 0) {
        u32 start = slot;
        u32 i = slot;
        do {
            i = NextSlot(i);
            u32 c = slots[i];
            if (c == 0xffffffff)
                continue;
            if (c < count) {
                count = c;
                slot = i;
                break;
            }
        } while (i != start);
    }
    slots[slot] = count + 1;

    f32 spread = RandomUnit() * 0.7853982f - 0.3926991f;
    f32 angle = g_unk_7101a715b8 * slot + spread;
    f32 height = p->max_height;
    f32 z = RandomUnit() * (height + height) - height;
    f32 x = *speed * cosf(angle);
    f32 y = *speed * sinf(angle);
    dir->x = x;
    dir->y = y;
    dir->z = z;

    f32 inv = 1.0f / (sqrtf(dir->x * dir->x + dir->y * dir->y + dir->z * dir->z) + 0.0001f);
    dir->x *= inv;
    dir->y *= inv;
    dir->z *= inv;
}
