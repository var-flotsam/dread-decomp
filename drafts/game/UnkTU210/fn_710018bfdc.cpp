// Unit 210 of tools/tu_map.py: particle emitters. Builds one textured quad (four vertices) per live
// particle into the shared vertex buffer, facing along a direction. Names and layouts are placeholders.
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
struct UnkVertex_l18bfdc {
    CVector3D pos;
    CVector2D uv;
    f32 r, g, b, a;
};
static_assert(sizeof(UnkVertex_l18bfdc) == 0x24, "UnkVertex_l18bfdc layout");

struct UnkEmitter_l18bfdc {
    u8 pad_0x0[0x88];
    CVector3D* positions;  // 0x88
    u8 pad_0x90[0x10];
    CVector3D rot;  // 0xa0
    u8 pad_0xac[0x1c];
    f32 field_0xc8;
    f32 field_0xcc;
    u8 pad_0xd0[0x20];
    f32 field_0xf0;
    f32 field_0xf4;
    u8 pad_0xf8[0x48];
    f32 r, g, b;  // 0x140
    u8 pad_0x14c[0x1c];
    f32 a;  // 0x168
    u8 pad_0x16c[0x1c];
    CVector2D uvOffset;  // 0x188
    u8 pad_0x190[0x18];
    f32 angle;  // 0x1a8
    u8 pad_0x1ac[0x1c];
    CVector2D uvScale;    // 0x1c8
    CVector2D* uvScales;  // 0x1d0, per particle when set
    u8 pad_0x1d8[0x170];
    s32 flags;               // 0x348
    CVector2D corners[4];    // 0x34c
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
static_assert(__builtin_offsetof(UnkEmitter_l18bfdc, flags) == 0x348, "flags");
static_assert(__builtin_offsetof(UnkEmitter_l18bfdc, count) == 0x390, "count");
static_assert(__builtin_offsetof(UnkEmitter_l18bfdc, field_0x43a) == 0x43a, "field_0x43a");

void fn_710018bfdc(UnkEmitter_l18bfdc* self, const UnkVec3f* direction) {
    CVector3D side;
    side.x = 1.0f;
    side.y = 0.0f;
    side.z = 0.0f;
    CVector3D fwd;
    fwd.x = 0.0f;
    fwd.y = 1.0f;
    fwd.z = 0.0f;
    UnkVertex_l18bfdc* vertices = (UnkVertex_l18bfdc*)g_unk_716e34c3d0;
    UnkVec3f dir = *direction;
    if (!self->field_0x43a) {
        dir = TransformDirectionByMatrix3x3(self->field_0x3f0, dir);
        f32 inv = 1.0f / (sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z) + 0.0001f);
        dir.x *= inv;
        dir.y *= inv;
        dir.z *= inv;
    }
    f32 width = self->width;
    f32 height = self->height;
    s32 flags = self->flags;
    fn_710016fa84((UnkOwner_710016fa84*)self, (const CVector3D*)&dir, &self->rot, &side, &fwd);

    // Bit 1 scales the size, bit 2 adds to it.
    f32 w = width * (f32)(flags & 2) * (self->field_0xc8 + self->field_0xf0) +
            width * (f32)(flags & 1) * (self->field_0xc8 * self->field_0xf0);
    f32 h = height * (f32)(flags & 2) * (self->field_0xcc + self->field_0xf4) +
            height * (f32)(flags & 1) * (self->field_0xcc * self->field_0xf4);
    CVector3D up = fwd * w;
    CVector3D right = side * h;
    CVector3D c0 = up * self->corners[0].x + right * self->corners[0].y;
    CVector3D c1 = up * self->corners[1].x + right * self->corners[1].y;
    CVector3D c2 = up * self->corners[2].x + right * self->corners[2].y;
    CVector3D c3 = up * self->corners[3].x + right * self->corners[3].y;

    f32 angle = self->angle;
    f32 s = sinf(angle);
    f32 c = cosf(angle);
    // The texture square's corners turned by the angle (v runs the other way).
    CVector2D uv0 = CVector2D{c * 0.5f - s * 0.5f, s * -0.5f + c * -0.5f} - self->uvOffset;
    CVector2D uv1 = CVector2D{c * -0.5f - s * 0.5f, s * 0.5f + c * -0.5f} - self->uvOffset;
    CVector2D uv2 = CVector2D{c * 0.5f - s * -0.5f, s * -0.5f + c * 0.5f} - self->uvOffset;
    CVector2D uv3 = CVector2D{c * -0.5f - s * -0.5f, s * 0.5f + c * 0.5f} - self->uvOffset;

    u32 count = self->count;
    UnkVertex_l18bfdc* v = vertices;
    for (u32 i = 0; i < count; i++, v += 4) {
        const CVector3D& p = self->positions[self->indices[i]];
        v[0].pos = c0 + p;
        v[1].pos = c1 + p;
        v[2].pos = c2 + p;
        v[3].pos = c3 + p;
        v[0].r = v[1].r = v[2].r = v[3].r = self->r;
        v[0].g = v[1].g = v[2].g = v[3].g = self->g;
        v[0].b = v[1].b = v[2].b = v[3].b = self->b;
        v[0].a = v[1].a = v[2].a = v[3].a = self->a;
        u32 index = self->indices[i];
        CVector2D* scales = self->uvScales;
        const CVector2D* scale = scales ? &scales[index] : &self->uvScale;
        v[0].uv.x = uv0.x * scale->x + 0.5f;
        v[0].uv.y = uv0.y * scale->y + 0.5f;
        v[1].uv.x = uv1.x * scale->x + 0.5f;
        v[1].uv.y = uv1.y * scale->y + 0.5f;
        v[2].uv.x = uv2.x * scale->x + 0.5f;
        v[2].uv.y = uv2.y * scale->y + 0.5f;
        v[3].uv.x = uv3.x * scale->x + 0.5f;
        v[3].uv.y = uv3.y * scale->y + 0.5f;
    }
}
