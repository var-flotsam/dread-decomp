// A unit with no known name (unit 210): writes one camera-facing quad per live particle into the frame's
// vertex buffer: corners from the emitter's size and corner table, rotated UVs, the particle's colour.
// Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector3D;

struct UnkOwner_710016fa84;

struct UnkQuadVertex_18ddb8 {
    UnkVec3f pos;
    f32 uv[2];
    UnkVec3f color;
    u32 field_0x20;
};
static_assert(sizeof(UnkQuadVertex_18ddb8) == 0x24, "");

struct UnkQuadCorner_18ddb8 {
    f32 up;
    f32 side;
};

struct UnkEmitter_18ddb8 {
    u8 pad_0x0[0x88];
    CVector3D* positions;
    u8 pad_0x90[0x10];
    CVector3D rotation;
    u8 pad_0xac[0x1c];
    f32 field_0xc8;
    f32 field_0xcc;
    u8 pad_0xd0[0x20];
    f32 field_0xf0;
    f32 field_0xf4;
    u8 pad_0xf8[0x48];
    UnkVec3f defaultColor;
    u8 pad_0x14c[4];
    UnkVec3f* colors;
    u8 pad_0x158[0x10];
    u32 field_0x168;
    u8 pad_0x16c[0x1c];
    f32 uvOffset[2];
    u8 pad_0x190[0x18];
    f32 uvAngle;
    u8 pad_0x1ac[0x1c];
    f32 uvScale[2];
    u8 pad_0x1d0[0x178];
    s32 sizeFlags;
    UnkQuadCorner_18ddb8 corners[4];
    u8 pad_0x36c[8];
    f32 sizeX;
    f32 sizeY;
    u8 pad_0x37c[0x14];
    u32 count;
    u32* indices;
    u8 pad_0x3a0[0x50];
    UnkMatrix44 matrix;
    u8 pad_0x430[0xa];
    bool field_0x43a;
};
static_assert(__builtin_offsetof(UnkEmitter_18ddb8, sizeFlags) == 0x348, "");
static_assert(__builtin_offsetof(UnkEmitter_18ddb8, count) == 0x390, "");
static_assert(__builtin_offsetof(UnkEmitter_18ddb8, matrix) == 0x3f0, "");
static_assert(__builtin_offsetof(UnkEmitter_18ddb8, field_0x43a) == 0x43a, "");

extern void* g_unk_716e34c3d0;

void fn_710016fa84(UnkOwner_710016fa84* self, const CVector3D* up, const CVector3D* rot,
                   CVector3D* side, CVector3D* fwd);

void fn_710018ddb8(UnkEmitter_18ddb8* self, const UnkVec3f* dir) {
    UnkQuadVertex_18ddb8* out = (UnkQuadVertex_18ddb8*)g_unk_716e34c3d0;
    CVector3D side;
    side.x = 1.0f;
    side.y = 0.0f;
    side.z = 0.0f;
    CVector3D fwd;
    fwd.x = 0.0f;
    fwd.y = 1.0f;
    fwd.z = 0.0f;
    UnkVec3f up = *dir;
    if (!self->field_0x43a) {
        up = TransformDirectionByMatrix3x3(self->matrix, up);
        f32 inv = 1.0f / (sqrtf(up.x * up.x + up.y * up.y + up.z * up.z) + 0.0001f);
        up.x = up.x * inv;
        up.y = up.y * inv;
        up.z = up.z * inv;
    }
    f32 sizeX = self->sizeX;
    f32 sizeY = self->sizeY;
    s32 flags = self->sizeFlags;
    fn_710016fa84((UnkOwner_710016fa84*)self, (const CVector3D*)&up, &self->rotation, &side, &fwd);

    f32 baseX = self->field_0xc8;
    f32 baseY = self->field_0xcc;
    CVector3D forward = fwd;
    f32 angle = self->uvAngle;
    f32 scaleX = self->field_0xf0;
    f32 scaleY = self->field_0xf4;
    CVector3D right = side;
    UnkQuadCorner_18ddb8 c0 = self->corners[0];
    UnkQuadCorner_18ddb8 c1 = self->corners[1];
    UnkQuadCorner_18ddb8 c2 = self->corners[2];
    UnkQuadCorner_18ddb8 c3 = self->corners[3];
    f32 s = sinf(angle);
    f32 c = cosf(angle);

    u32 count = self->count;
    if (count == 0)
        return;

    f32 additive = flags & 2;
    f32 multiplied = flags & 1;
    f32 width = sizeX * additive * (baseX + scaleX) + sizeX * multiplied * (baseX * scaleX);
    f32 height = sizeY * additive * (baseY + scaleY) + sizeY * multiplied * (baseY * scaleY);
    CVector3D u = {forward.x * width, forward.y * width, forward.z * width};
    CVector3D r = {right.x * height, right.y * height, right.z * height};
    CVector3D p0 = {u.x * c0.up + r.x * c0.side, u.y * c0.up + r.y * c0.side, u.z * c0.up + r.z * c0.side};
    CVector3D p1 = {u.x * c1.up + r.x * c1.side, u.y * c1.up + r.y * c1.side, u.z * c1.up + r.z * c1.side};
    CVector3D p2 = {u.x * c2.up + r.x * c2.side, u.y * c2.up + r.y * c2.side, u.z * c2.up + r.z * c2.side};
    CVector3D p3 = {u.x * c3.up + r.x * c3.side, u.y * c3.up + r.y * c3.side, u.z * c3.up + r.z * c3.side};

    // The UV square's corners (+-0.5), rotated by the UV angle, then offset and scaled about its centre.
    f32 u0 = (0.5f * c - 0.5f * s - self->uvOffset[0]) * self->uvScale[0] + 0.5f;
    f32 v0 = (-0.5f * s + -0.5f * c - self->uvOffset[1]) * self->uvScale[1] + 0.5f;
    f32 u1 = (-0.5f * c - 0.5f * s - self->uvOffset[0]) * self->uvScale[0] + 0.5f;
    f32 v1 = (0.5f * s + -0.5f * c - self->uvOffset[1]) * self->uvScale[1] + 0.5f;
    f32 u2 = (0.5f * c - -0.5f * s - self->uvOffset[0]) * self->uvScale[0] + 0.5f;
    f32 v2 = (-0.5f * s + 0.5f * c - self->uvOffset[1]) * self->uvScale[1] + 0.5f;
    f32 u3 = (-0.5f * c - -0.5f * s - self->uvOffset[0]) * self->uvScale[0] + 0.5f;
    f32 v3 = (0.5f * s + 0.5f * c - self->uvOffset[1]) * self->uvScale[1] + 0.5f;

    for (u32 i = 0; i < count; i++) {
        const CVector3D& p = self->positions[self->indices[i]];
        out[0].pos = UnkVec3f(p0.x + p.x, p0.y + p.y, p0.z + p.z);
        out[1].pos = UnkVec3f(p1.x + p.x, p1.y + p.y, p1.z + p.z);
        out[2].pos = UnkVec3f(p2.x + p.x, p2.y + p.y, p2.z + p.z);
        out[3].pos = UnkVec3f(p3.x + p.x, p3.y + p.y, p3.z + p.z);
        const UnkVec3f* source = self->colors != nullptr ? &self->colors[self->indices[i]] : &self->defaultColor;
        UnkVec3f color = *source;
        out[0].color = color;
        out[1].color = color;
        out[2].color = color;
        out[3].color = color;
        out[0].field_0x20 = self->field_0x168;
        out[1].field_0x20 = self->field_0x168;
        out[2].field_0x20 = self->field_0x168;
        out[3].field_0x20 = self->field_0x168;
        out[0].uv[0] = u0;
        out[0].uv[1] = v0;
        out[1].uv[0] = u1;
        out[1].uv[1] = v1;
        out[2].uv[0] = u2;
        out[2].uv[1] = v2;
        out[3].uv[0] = u3;
        out[3].uv[1] = v3;
        out += 4;
    }
}
