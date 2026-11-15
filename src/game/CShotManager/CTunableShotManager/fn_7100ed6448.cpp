// CShotManager::CTunableShotManager unit: picks a free shot (else the live one with the largest
// travelled distance) and sets a value on it, marking it live. Names and layouts are placeholders.
#include "types.h"

struct UnkShot_ed6448 {
    u8 pad_0x0[0x8];
    u32 field_0x8;
    u8 pad_0xc[0xac - 0xc];
    f32 field_0xac;
    f32 field_0xb0;
    f32 field_0xb4;
    u8 pad_0xb8[0xd0 - 0xb8];
    f32 field_0xd0;
    f32 field_0xd4;
    f32 field_0xd8;
    u8 pad_0xdc[0x14a - 0xdc];
    bool field_0x14a;
    u8 pad_0x14b[0x290 - 0x14b];
};

struct UnkShotManager_ed6448 {
    u8 pad_0x0[0x8];
    UnkShot_ed6448* shots;
    u32 shotCount;
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_7100ed6448(UnkShotManager_ed6448* self, u32 value) {
    UnkShot_ed6448* best = nullptr;
    UnkShot_ed6448* free = nullptr;
    f32 bestDist = -1.0f;
    for (UnkShot_ed6448 *it = self->shots, *end = self->shots + self->shotCount; it != end; ++it) {
        if (!it->field_0x14a) {
            free = it;
            break;
        }
        f32 dx = it->field_0xd0 - it->field_0xac;
        f32 dy = it->field_0xd4 - it->field_0xb0;
        f32 dz = it->field_0xd8 - it->field_0xb4;
        f32 d = dx * dx + dy * dy + dz * dz;
        if (d > bestDist) {
            bestDist = d;
            best = it;
        }
    }
    UnkShot_ed6448* shot = free ? free : best;
    if (shot) {
        shot->field_0x8 = value;
        shot->field_0x14a = true;
    }
}
