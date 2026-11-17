// Unit 210 of tools/tu_map.py: particle emitters. Builds one textured quad (four vertices) per live
// particle into the shared vertex buffer, with per-particle sizes and texture offsets.
// Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/math/CVectorOps.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkOwner_710016fa84;

void fn_710016fa84(UnkOwner_710016fa84* self, const CVector3D* up, const CVector3D* rot,
                   CVector3D* side, CVector3D* fwd);

extern void* g_unk_716e34c3d0;

// One vertex of a particle quad: position, texture coordinate and colour.
struct UnkVertex_l17fef4 {
    f32 pos[3];
    f32 uv[2];
    f32 color[4];
};
static_assert(sizeof(UnkVertex_l17fef4) == 0x24, "UnkVertex_l17fef4 layout");

struct UnkEmitter_l17fef4 {
    u8 pad_0x0[0x88];
    CVector3D* positions;  // 0x88
    u8 pad_0x90[0x10];
    CVector3D rot;  // 0xa0
    u8 pad_0xac[0x1c];
    CVector3D size;    // 0xc8
    u8 pad_0xd4[0x4];
    CVector3D* sizes;  // 0xd8, per particle when set
    u8 pad_0xe0[0x10];
    CVector3D scale;    // 0xf0
    u8 pad_0xfc[0x4];
    CVector3D* scales;  // 0x100, per particle when set
    u8 pad_0x108[0x38];
    f32 r, g, b;  // 0x140
    u8 pad_0x14c[0x1c];
    f32 a;  // 0x168
    u8 pad_0x16c[0x1c];
    CVector2D uvOffset;    // 0x188
    CVector2D* uvOffsets;  // 0x190, per particle when set
    u8 pad_0x198[0x10];
    f32 angle;  // 0x1a8
    u8 pad_0x1ac[0x1c];
    CVector2D uvScale;  // 0x1c8
    u8 pad_0x1d0[0x178];
    s32 flags;             // 0x348
    CVector2D corners[4];  // 0x34c
    u8 pad_0x36c[0x8];
    f32 width;   // 0x374
    f32 height;  // 0x378
    u8 pad_0x37c[0x14];
    u32 count;     // 0x390
    u32* indices;  // 0x398
    u8 pad_0x3a0[0x50];
    UnkMatrix44 field_0x3f0;
    u8 pad_0x430[0xa];
    bool field_0x43a;
};
static_assert(__builtin_offsetof(UnkEmitter_l17fef4, sizes) == 0xd8, "sizes");
static_assert(__builtin_offsetof(UnkEmitter_l17fef4, scales) == 0x100, "scales");
static_assert(__builtin_offsetof(UnkEmitter_l17fef4, flags) == 0x348, "flags");
static_assert(__builtin_offsetof(UnkEmitter_l17fef4, count) == 0x390, "count");
static_assert(__builtin_offsetof(UnkEmitter_l17fef4, field_0x43a) == 0x43a, "field_0x43a");

// out = p + up * corner.x + right * corner.y, one component at a time.
inline void SetCorner(f32* out, const CVector3D& p, const CVector3D& up, const CVector3D& right,
                      const CVector2D& corner) {
    out[0] = p.x + up.x * corner.x + right.x * corner.y;
    out[1] = p.y + up.y * corner.x + right.y * corner.y;
    out[2] = p.z + up.z * corner.x + right.z * corner.y;
}

void fn_710017fef4(UnkEmitter_l17fef4* self, const UnkVec3f* direction) {
    CVector3D side = {0.0f, 0.0f, 0.0f};
    CVector3D fwd = {0.0f, 0.0f, 0.0f};
    UnkVertex_l17fef4* v = (UnkVertex_l17fef4*)g_unk_716e34c3d0;
    UnkVec3f dir = *direction;
    if (!self->field_0x43a) {
        dir = fn_7100077c04(self->field_0x3f0, dir);
        f32 inv = 1.0f / (sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z) + 0.0001f);
        dir.x *= inv;
        dir.y *= inv;
        dir.z *= inv;
    }
    fn_710016fa84((UnkOwner_710016fa84*)self, (const CVector3D*)&dir, &self->rot, &side, &fwd);

    f32 angle = self->angle;
    f32 width = self->width;
    f32 height = self->height;
    s32 flags = self->flags;
    f32 s = sinf(angle);
    f32 c = cosf(angle);
    // The texture square's corners turned by the angle (v runs the other way).
    CVector2D uv0 = {c * 0.5f - s * 0.5f, s * -0.5f + c * -0.5f};
    CVector2D uv1 = {c * -0.5f - s * 0.5f, s * 0.5f + c * -0.5f};
    CVector2D uv2 = {c * 0.5f - s * -0.5f, s * -0.5f + c * 0.5f};
    CVector2D uv3 = {c * -0.5f - s * -0.5f, s * 0.5f + c * 0.5f};

    for (u32 i = 0; i < self->count; i++, v += 4) {
        u32 index = self->indices[i];
        const CVector3D* size = self->sizes ? &self->sizes[index] : &self->size;
        const CVector3D* scale = self->scales ? &self->scales[index] : &self->scale;
        // Bit 1 scales the size, bit 2 adds to it.
        f32 w = width * (f32)(flags & 2) * (size->x + scale->x) +
                width * (f32)(flags & 1) * (size->x * scale->x);
        f32 h = height * (f32)(flags & 2) * (size->y + scale->y) +
                height * (f32)(flags & 1) * (size->y * scale->y);
        CVector3D up = fwd * w;
        CVector3D right = side * h;
        const CVector3D& p = self->positions[index];
        SetCorner(v[0].pos, p, up, right, self->corners[0]);
        SetCorner(v[1].pos, p, up, right, self->corners[1]);
        SetCorner(v[2].pos, p, up, right, self->corners[2]);
        SetCorner(v[3].pos, p, up, right, self->corners[3]);
        v[0].color[0] = v[1].color[0] = v[2].color[0] = v[3].color[0] = self->r;
        v[0].color[1] = v[1].color[1] = v[2].color[1] = v[3].color[1] = self->g;
        v[0].color[2] = v[1].color[2] = v[2].color[2] = v[3].color[2] = self->b;
        v[0].color[3] = v[1].color[3] = v[2].color[3] = v[3].color[3] = self->a;
        const CVector2D* offset = self->uvOffsets ? &self->uvOffsets[self->indices[i]] : &self->uvOffset;
        v[0].uv[0] = (uv0.x - offset->x) * self->uvScale.x + 0.5f;
        v[0].uv[1] = (uv0.y - offset->y) * self->uvScale.y + 0.5f;
        v[1].uv[0] = (uv1.x - offset->x) * self->uvScale.x + 0.5f;
        v[1].uv[1] = (uv1.y - offset->y) * self->uvScale.y + 0.5f;
        v[2].uv[0] = (uv2.x - offset->x) * self->uvScale.x + 0.5f;
        v[2].uv[1] = (uv2.y - offset->y) * self->uvScale.y + 0.5f;
        v[3].uv[0] = (uv3.x - offset->x) * self->uvScale.x + 0.5f;
        v[3].uv[1] = (uv3.y - offset->y) * self->uvScale.y + 0.5f;
    }
}
