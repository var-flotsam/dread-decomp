#include "base/math/CVector4D.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::math::CVector4D;

struct UnkVec3_b0191 {
    f32 x, y, z;
};

UnkVec3_b0191* fn_710021a04c(UnkRenderManager* mgr);
UnkVec3_b0191* fn_710021a060(UnkRenderManager* mgr);
void fn_7100219f6c(UnkRenderManager* mgr, void* a, void* b, u32 flag);

static inline bool NearZero_b0191(f32 t) {
    return t - 0.001f <= 0.0f && t + 0.001f >= 0.0f;
}

struct UnkObj98d24 {
    u8 pad_0x0[0x17c];
    UnkVec3_b0191 first;   // 0x17c
    UnkVec3_b0191 second;  // 0x188
    u8 pad_0x194[0x370 - 0x194];
    f32 flag;  // 0x370
    u8 pad_0x374[0x3e4 - 0x374];
    f32 t;  // 0x3e4
    u8 pad_0x3e8[0x43a - 0x3e8];
    u8 clear;  // 0x43a
};

void fn_7100f98d24(UnkObj98d24* self, f32 t, UnkVec3_b0191* v) {
    self->t = t;
    self->clear = 0;
    self->flag = NearZero_b0191(t) ? 1.0f : 0.0f;
    UnkVec3_b0191* p = fn_710021a04c(g_unk_7101d47300);
    self->first.x = p->x;
    self->first.y = p->y;
    self->first.z = p->z;
    self->second.x = v->x;
    self->second.y = v->y;
    self->second.z = v->z;
}

struct UnkObj98dc4 {
    u8 pad_0x0[0x194];
    UnkVec3_b0191 first;   // 0x194
    UnkVec3_b0191 second;  // 0x1a0
    u8 pad_0x1ac[0x374 - 0x1ac];
    f32 flag;  // 0x374
    u8 pad_0x378[0x3e8 - 0x378];
    f32 t;  // 0x3e8
    u8 pad_0x3ec[0x43b - 0x3ec];
    u8 clear;  // 0x43b
};

void fn_7100f98dc4(UnkObj98dc4* self, f32 t, UnkVec3_b0191* v) {
    self->t = t;
    self->clear = 0;
    self->flag = NearZero_b0191(t) ? 1.0f : 0.0f;
    UnkVec3_b0191* p = fn_710021a060(g_unk_7101d47300);
    self->first.x = p->x;
    self->first.y = p->y;
    self->first.z = p->z;
    self->second.x = v->x;
    self->second.y = v->y;
    self->second.z = v->z;
}

struct UnkObj990e0 {
    u8 pad_0x0[0x2e0];
    CVector4D field_0x2e0;
    CVector4D field_0x2f0;
    CVector4D field_0x300;
    u8 pad_0x310[0x31c - 0x310];
    u8 flag;  // 0x31c
    u8 pad_0x31d[0x3b0 - 0x31d];
    f32 near;  // 0x3b0
    u8 pad_0x3b4[0x424 - 0x3b4];
    f32 t;  // 0x424
    u8 pad_0x428[0x44a - 0x428];
    u8 clear;  // 0x44a
};

void fn_7100f990e0(UnkObj990e0* self, f32 t, CVector4D* v) {
    self->flag = 1;
    self->t = t;
    self->clear = 0;
    self->near = NearZero_b0191(t) ? 1.0f : 0.0f;
    fn_710007b518(&self->field_0x2f0, &self->field_0x2e0);
    fn_710007b518(&self->field_0x300, v);
}

struct UnkObj98094 {
    u8 pad_0x0[0x3c];
    u32 a[2];  // 0x3c
    u32 b[2];  // 0x44
    f32 c[2];  // 0x4c
    u8 pad_0x54[0x350 - 0x54];
    f32 flag;  // 0x350
    u8 pad_0x354[0x3c4 - 0x354];
    f32 t;  // 0x3c4
    u8 pad_0x3c8[0x432 - 0x3c8];
    u8 clear;  // 0x432
};

void fn_7100f98094(UnkObj98094* self, f32 t, f32* v) {
    self->t = t;
    self->clear = 0;
    self->flag = NearZero_b0191(t) ? 1.0f : 0.0f;
    fn_7100219f6c(g_unk_7101d47300, &self->a[0], &self->a[1], 0);
    self->b[0] = self->a[0];
    self->b[1] = self->a[1];
    self->c[0] = v[0];
    self->c[1] = v[1];
}
