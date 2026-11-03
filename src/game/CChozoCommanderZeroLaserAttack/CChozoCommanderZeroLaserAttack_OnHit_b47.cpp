// CChozoCommanderZeroLaserAttack unit (tools/tu_map.py unit 659): when the hit's direction is within
// about 25 degrees of the x axis (|dot| > 0.9), the owner has a character class, the state at +0x90 is
// clear and the value at +0x58 has reached the tunable's +0xf8, runs fn_7100444378. Names and layouts
// are placeholders.
#include <math.h>
#include "game/UnkActor.h"
#include "types.h"

struct UnkVec3_b47 {
    f32 x, y, z;
};
// The dot product as a loop over the components (the original's order of loads and adds).
static inline f32 Dot_b47(const UnkVec3_b47& a, const UnkVec3_b47& b) {
    const f32* p = &a.x;
    const f32* q = &b.x;
    f32 s = p[0] * q[0];
    for (int i = 1; i < 3; ++i)
        s += p[i] * q[i];
    return s;
}

struct UnkZeroLaserTunable_b47 {
    u8 pad_0x0[0xf8];
    f32 field_0xf8;
};

struct UnkZeroLaserAttack_b47 {
    u8 pad_0x0[0x8];
    UnkActor* owner;                    // 0x8
    u8 pad_0x10[0x40];
    UnkZeroLaserTunable_b47* tunable;   // 0x50
    f32 field_0x58;                     // 0x58
    u8 pad_0x5c[0x34];
    u32 field_0x90;                     // 0x90
};

struct UnkZeroLaserHit_b47 {
    u8 pad_0x0[0x2c];
    UnkVec3_b47 dir;  // 0x2c
};

void fn_7100444378(UnkZeroLaserAttack_b47* self);

void fn_71004446d4(UnkZeroLaserAttack_b47* self, void* unused, const UnkZeroLaserHit_b47* hit) {
    const UnkVec3_b47 axis = {1.0f, 0.0f, 0.0f};
    if (fabsf(Dot_b47(hit->dir, axis)) > 0.9f && self->owner->field_0x220 && self->field_0x90 == 0 &&
        self->field_0x58 >= self->tunable->field_0xf8)
        fn_7100444378(self);
}
