// Unit 308: a timed distance range (min/max reach around a position). Names are placeholders.
#include "types.h"

extern "C" f32 sqrtf(f32);
extern char g_unk_7101a20220[];

struct UnkRange_b0166 {
    void* vtable;  // 0x0
    f32 reach;     // 0x8
    u8 pad_0xc[0xc];
    f32 pos[3];     // 0x18
    f32 near_a;     // 0x24
    f32 far_a;      // 0x28
    f32 near_b;     // 0x2c
    f32 far_b;      // 0x30
    s64 ticks;      // 0x38
    s32 scaled[3];  // 0x40
    u8 flag;        // 0x4c
    u8 flag2;       // 0x4d
};

void fn_7100216d30(UnkRange_b0166* self, f32 scale) {
    f32 v = self->far_b * scale;
    s64 t = (s64)(v * 10000.0f);
    self->ticks = t;
    self->far_a = v;
    self->reach = v;
    self->flag = t == 0;
}

bool fn_7100216ca4(UnkRange_b0166* self, UnkRange_b0166* other) {
    f32 d = __builtin_fabsf(other->pos[0] - self->pos[0]);
    d += other->reach;
    f32 a = self->near_a;
    if (!(a <= d))
        return false;
    f32 b = self->far_a;
    if (!(b >= d))
        return false;
    d = __builtin_fabsf(other->pos[1] - self->pos[1]);
    d += other->reach;
    if (!(a <= d && b >= d))
        return false;
    d = other->reach + __builtin_fabsf(other->pos[2] - self->pos[2]);
    if (!(a <= d))
        return false;
    return b >= d;
}

void fn_710021688c(UnkRange_b0166* self, f32 radius, const f32* pos) {
    self->reach = radius;
    __builtin_memset(self->pad_0xc, 0, 0xc);
    self->vtable = g_unk_7101a20220 + 0x10;
    u32 bx = ((const u32*)pos)[0];
    u32 by = ((const u32*)pos)[1];
    u32 bz = ((const u32*)pos)[2];
    ((u32*)self->pos)[0] = bx;
    ((u32*)self->pos)[1] = by;
    ((u32*)self->pos)[2] = bz;
    f32 x, y, z;
    __builtin_memcpy(&x, &bx, 4);
    __builtin_memcpy(&y, &by, 4);
    __builtin_memcpy(&z, &bz, 4);
    if (radius >= 0.0f) {
        self->near_a = 0.0f;
        self->far_a = radius;
        self->near_b = 0.0f;
        self->far_b = radius;
    }
    s64 t = (s64)(radius * 10000.0f);
    self->ticks = t;
    self->flag = t == 0;
    self->scaled[0] = (s32)(x * 10.0f);
    self->scaled[1] = (s32)(y * 10.0f);
    self->scaled[2] = (s32)(z * 10.0f);
    self->flag2 = 0;
}

static inline f32 Widen(f32* lo, f32* hi, f32 d) {
    f32* slot = nullptr;
    f32 top = *hi;
    if (!(*lo <= top)) {
        slot = hi;
        top = d;
        *lo = d;
    } else if (*lo > d) {
        slot = lo;
    } else if (top < d) {
        slot = hi;
        top = d;
    }
    if (slot)
        *slot = d;
    return top;
}

static inline void Apply(UnkRange_b0166* self, f32 dist) {
    f32 d = dist - self->reach;
    d = !(d < 0.0f) ? d : 0.0f;
    f32 top = Widen(&self->near_a, &self->far_a, d);
    Widen(&self->near_b, &self->far_b, d);
    s64 t = (s64)(top * 10000.0f);
    self->ticks = t;
    self->reach = top;
    self->flag = t == 0;
}

void fn_71002169e0(UnkRange_b0166* self, const f32* p) {
    f32 dx = p[0] - self->pos[0];
    f32 dy = p[1] - self->pos[1];
    f32 dz = p[2] - self->pos[2];
    Apply(self, sqrtf(dx * dx + dy * dy + dz * dz));
}

void fn_7100216b00(UnkRange_b0166* self, const f32* p, f32 margin) {
    f32 dx = __builtin_fabsf(p[0] - self->pos[0]) + margin;
    f32 dy = __builtin_fabsf(p[1] - self->pos[1]) + margin;
    f32 dz = __builtin_fabsf(p[2] - self->pos[2]) + margin;
    Apply(self, sqrtf(dy * dy + dx * dx + dz * dz));
}
