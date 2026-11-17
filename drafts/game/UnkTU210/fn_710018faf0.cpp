// A unit with no known name (unit 210): builds the camera-facing quads of a GPU particle emitter. One
// variant per combination of the emitter's per-particle attribute buffers (fn_7100170268 picks it); this
// is the one with none of them. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkTU54.h"
#include "types.h"
#include <math.h>

using base::math::CVector3D;

struct UnkOwner_710016fa84;
void fn_710016fa84(UnkOwner_710016fa84* self, const CVector3D* up, const CVector3D* rot, CVector3D* side,
                   CVector3D* fwd);

struct UnkBillboardVertex {
    f32 x, y, z;
    f32 u, v;
    f32 r, g, b, a;
};

struct UnkBillboardQuad {
    UnkBillboardVertex v[4];
};

struct UnkBillboardEmitter {
    u8 pad_0x0[0x88];
    CVector3D* positions;  // 0x88
    u8 pad_0x90[0xa0 - 0x90];
    CVector3D rotation;  // 0xa0
    u8 pad_0xac[0xc8 - 0xac];
    f32 scale_x;  // 0xc8
    f32 scale_y;  // 0xcc
    u8 pad_0xd0[0xf0 - 0xd0];
    f32 grow_x;  // 0xf0
    f32 grow_y;  // 0xf4
    u8 pad_0xf8[0x140 - 0xf8];
    f32 color_r;  // 0x140
    f32 color_g;  // 0x144
    f32 color_b;  // 0x148
    u8 pad_0x14c[0x168 - 0x14c];
    f32 alpha;  // 0x168
    u8 pad_0x16c[0x188 - 0x16c];
    f32 uv_offset_u;  // 0x188
    f32 uv_offset_v;  // 0x18c
    u8 pad_0x190[0x1a8 - 0x190];
    f32 angle;  // 0x1a8
    u8 pad_0x1ac[0x1c8 - 0x1ac];
    f32 uv_scale_u;  // 0x1c8
    f32 uv_scale_v;  // 0x1cc
    u8 pad_0x1d0[0x348 - 0x1d0];
    u32 size_mode;  // 0x348
    f32 corners[4][2];  // 0x34c
    u8 pad_0x36c[0x374 - 0x36c];
    f32 size_x;  // 0x374
    f32 size_y;  // 0x378
    u8 pad_0x37c[0x390 - 0x37c];
    u32 count;  // 0x390
    u8 pad_0x394[4];
    u32* indices;  // 0x398
    u8 pad_0x3a0[0x3f0 - 0x3a0];
    UnkMatrix44 world;  // 0x3f0
    u8 pad_0x430[0x43a - 0x430];
    bool world_space;  // 0x43a
};

extern UnkBillboardQuad* g_unk_716e34c3d0;

void fn_710018faf0(UnkBillboardEmitter* self, const UnkVec3f& view) {
    CVector3D side;
    side.x = 1.0f;
    side.y = 0.0f;
    side.z = 0.0f;
    UnkBillboardQuad* quads = g_unk_716e34c3d0;
    CVector3D fwd;
    fwd.x = 0.0f;
    fwd.y = 1.0f;
    fwd.z = 0.0f;
    UnkVec3f dir = view;
    if (!self->world_space) {
        dir = fn_7100077c04(self->world, dir);
        f32 len = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
        f32 inv = 1.0f / (len + 0.0001f);
        dir.x = dir.x * inv;
        dir.y = inv * dir.y;
        dir.z = inv * dir.z;
    }
    f32 size_x = self->size_x;
    f32 size_y = self->size_y;
    s32 mode = self->size_mode;
    fn_710016fa84(reinterpret_cast<UnkOwner_710016fa84*>(self), reinterpret_cast<const CVector3D*>(&dir),
                  &self->rotation, &side, &fwd);
    f32 scale_x = self->scale_x;
    f32 scale_y = self->scale_y;
    f32 angle = self->angle;
    f32 grow_x = self->grow_x;
    f32 grow_y = self->grow_y;
    f32 fx = fwd.x, fy = fwd.y, fz = fwd.z;
    f32 sx = side.x, sy = side.y, sz = side.z;
    f32 k0u = self->corners[0][0], k0v = self->corners[0][1];
    f32 k1u = self->corners[1][0], k1v = self->corners[1][1];
    f32 k2u = self->corners[2][0], k2v = self->corners[2][1];
    f32 k3u = self->corners[3][0], k3v = self->corners[3][1];
    f32 sine = sinf(angle);
    f32 cosine = cosf(angle);
    u32 count = self->count;
    if (count == 0)
        return;

    f32 add = static_cast<f32>(mode & 2);
    f32 mul = static_cast<f32>(mode & 1);
    f32 w = size_x * add * (scale_x + grow_x) + size_x * mul * (scale_x * grow_x);
    f32 h = size_y * add * (scale_y + grow_y) + size_y * mul * (scale_y * grow_y);
    f32 ax = fx * w, ay = w * fy, az = w * fz;
    f32 bx = h * sx, by = h * sy, bz = h * sz;

    f32 c0x = ax * k0u + bx * k0v;
    f32 c0y = ay * k0u + by * k0v;
    f32 c0z = az * k0u + bz * k0v;
    f32 c1x = ax * k1u + bx * k1v;
    f32 c1y = ay * k1u + by * k1v;
    f32 c1z = az * k1u + bz * k1v;
    f32 c2x = ax * k2u + bx * k2v;
    f32 c2y = ay * k2u + by * k2v;
    f32 c2z = az * k2u + bz * k2v;
    f32 c3x = ax * k3u + bx * k3v;
    f32 c3y = ay * k3u + by * k3v;
    f32 c3z = az * k3u + bz * k3v;

    // The texture's corners, turned by the angle about the centre.
    f32 cp = cosine * 0.5f;
    f32 cm = cosine * -0.5f;
    f32 sp = sine * 0.5f;
    f32 sm = sine * -0.5f;
    f32 u0 = self->uv_scale_u * (cp - sp - self->uv_offset_u) + 0.5f;
    f32 v0 = (sm + cm - self->uv_offset_v) * self->uv_scale_v + 0.5f;
    f32 u1 = self->uv_scale_u * (cm - sp - self->uv_offset_u) + 0.5f;
    f32 v1 = (sp + cm - self->uv_offset_v) * self->uv_scale_v + 0.5f;
    f32 u2 = self->uv_scale_u * (cp - sm - self->uv_offset_u) + 0.5f;
    f32 v2 = (sm + cp - self->uv_offset_v) * self->uv_scale_v + 0.5f;
    f32 u3 = self->uv_scale_u * (cm - sm - self->uv_offset_u) + 0.5f;
    f32 v3 = (sp + cp - self->uv_offset_v) * self->uv_scale_v + 0.5f;

    for (u32 i = 0; i != count; ++i) {
        UnkBillboardQuad& q = quads[i];
        const CVector3D& p = self->positions[self->indices[i]];
        q.v[0].x = c0x + p.x;
        q.v[0].y = c0y + p.y;
        q.v[0].z = c0z + p.z;
        q.v[1].x = c1x + p.x;
        q.v[1].y = c1y + p.y;
        q.v[1].z = c1z + p.z;
        q.v[2].x = c2x + p.x;
        q.v[2].y = c2y + p.y;
        q.v[2].z = c2z + p.z;
        q.v[3].x = c3x + p.x;
        q.v[3].y = c3y + p.y;
        q.v[3].z = c3z + p.z;
        q.v[0].r = q.v[1].r = q.v[2].r = q.v[3].r = self->color_r;
        q.v[0].g = q.v[1].g = q.v[2].g = q.v[3].g = self->color_g;
        q.v[0].b = q.v[1].b = q.v[2].b = q.v[3].b = self->color_b;
        q.v[0].a = q.v[1].a = q.v[2].a = q.v[3].a = self->alpha;
        q.v[0].u = u0;
        q.v[0].v = v0;
        q.v[1].u = u1;
        q.v[1].v = v1;
        q.v[2].u = u2;
        q.v[2].v = v2;
        q.v[3].u = u3;
        q.v[3].v = v3;
    }
}
