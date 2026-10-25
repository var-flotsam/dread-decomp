// Tests whether the point (x, y, z) from the target's position (+0x44, +0x54, +0x64) offset along its
// matrix's third column by the scale at +0x14 and along the second column by the scale at +0x18 is
// blocked: calls fn_7100219588 on the global at g_unk_7101d47300 with it and returns whether the
// answer has bit 0 clear.
#include "base/UnkMatrix44.h"
#include "game/UnkRenderManager.h"
#include "types.h"

struct UnkTarget {
    u8 pad_0x0[0x38];
    u8 field_0x38[0xc];
    f32 field_0x44;
    u8 pad_0x48[0xc];
    f32 field_0x54;
    u8 pad_0x58[0xc];
    u32 field_0x64;
};

struct UnkSelf {
    u8 pad_0x0[0x14];
    f32 field_0x14;
    f32 field_0x18;
};

bool fn_7100219588(UnkRenderManager* self, const void* pos, u32 a, u32 b);  // src/game_lo/fn_7100219588.cpp

// NON_MATCHING(minor): the sums land in other registers (the original adds into s8-s10 with the products
// as first operand, a commuted add; one product is also kept in s5 instead of s0)
u32 fn_71002c1630(UnkSelf* self, void* unused, UnkTarget* target) {
    f32 x = target->field_0x44;
    f32 y = target->field_0x54;
    alignas(16) f32 point[4];
    __builtin_memcpy(&point[2], &target->field_0x64, 4);
    const UnkMatrix44* m = &fn_710002614c(reinterpret_cast<const UnkMatrix44*>(target->field_0x38));
    f32 z0 = m->m[2];
    f32 z1 = m->m[6];
    f32 z2 = m->m[10];
    f32 s = self->field_0x14;
    f32 o0 = z0 * s;
    f32 o1 = z1 * s;
    f32 o2 = z2 * s;
    f32 ax = x + o0;
    f32 ay = y + o1;
    f32 az = point[2] + o2;
    point[1] = ay;
    point[2] = az;
    m = &fn_710002614c(reinterpret_cast<const UnkMatrix44*>(target->field_0x38));
    f32 y0 = m->m[1];
    f32 y1 = m->m[5];
    f32 y2 = m->m[9];
    f32 t = self->field_0x18;
    f32 p0 = y0 * t;
    f32 p1 = y1 * t;
    f32 p2 = y2 * t;
    point[0] = ax + p0;
    point[1] = ay + p1;
    point[2] = az + p2;
    return ~fn_7100219588(g_unk_7101d47300, point, 0, 0) & 1;
}
