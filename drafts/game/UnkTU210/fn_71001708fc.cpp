// A unit with no known name (unit 210): writes one quad per live particle of an emitter into the frame's
// vertex buffer, each particle with its own rotation, size, colour, alpha, angle and texture transform.
// Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/math/CVectorOps.h"
#include "game/UnkTU54.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// A per-particle value, or one value for every particle when there is no array.
template <class T>
struct UnkPerParticle_l1708fc {
    T value;
    T* values;
    const T& Get(u32 i) const { return values ? values[i] : value; }
};

struct UnkColor_l1708fc {
    u32 rgb[3];
};

struct UnkEmitter_l1708fc {
    u8 pad_0x0[0x88];
    CVector3D* positions;  // 0x88
    u8 pad_0x90[0x10];
    UnkPerParticle_l1708fc<CVector3D> rotation;  // 0xa0
    u8 pad_0xb8[0x10];
    UnkPerParticle_l1708fc<CVector3D> sizeA;  // 0xc8
    u8 pad_0xe0[0x10];
    UnkPerParticle_l1708fc<CVector3D> sizeB;  // 0xf0
    u8 pad_0x108[0x38];
    UnkPerParticle_l1708fc<UnkColor_l1708fc> color;  // 0x140
    u8 pad_0x158[0x10];
    UnkPerParticle_l1708fc<u32> alpha;  // 0x168
    u8 pad_0x178[0x10];
    UnkPerParticle_l1708fc<CVector2D> uvPivot;  // 0x188
    u8 pad_0x198[0x10];
    UnkPerParticle_l1708fc<f32> angle;  // 0x1a8
    u8 pad_0x1b8[0x10];
    UnkPerParticle_l1708fc<CVector2D> uvScale;  // 0x1c8
    u8 pad_0x1d8[0x170];
    s32 sizeFlags;  // 0x348
    CVector2D corners[4];  // 0x34c
    u8 pad_0x36c[8];
    f32 scaleX;  // 0x374
    f32 scaleY;  // 0x378
    u8 pad_0x37c[0x14];
    u32 count;  // 0x390
    u32* indices;  // 0x398
    u8 pad_0x3a0[0x50];
    UnkMatrix44 field_0x3f0;
    u8 pad_0x430[0xa];
    bool field_0x43a;
};
static_assert(__builtin_offsetof(UnkEmitter_l1708fc, rotation) == 0xa0, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l1708fc, sizeA) == 0xc8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l1708fc, sizeB) == 0xf0, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l1708fc, color) == 0x140, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l1708fc, alpha) == 0x168, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l1708fc, uvPivot) == 0x188, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l1708fc, angle) == 0x1a8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l1708fc, uvScale) == 0x1c8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l1708fc, sizeFlags) == 0x348, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l1708fc, count) == 0x390, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l1708fc, field_0x3f0) == 0x3f0, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l1708fc, field_0x43a) == 0x43a, "layout");

struct UnkVertex_l1708fc {
    f32 pos[3];
    f32 uv[2];
    u32 color[4];
};

struct UnkQuad_l1708fc {
    UnkVertex_l1708fc v[4];
};

// The texture transform of one particle: its rotation and scale, and the pivot moved to the centre.
struct UnkUVTransform_l1708fc {
    f32 xx, xy, tx;
    f32 yx, yy, ty;
};

struct UnkOwner_710016fa84;
void fn_710016fa84(UnkOwner_710016fa84* self, const CVector3D* up, const CVector3D* rot,
                   CVector3D* side, CVector3D* fwd);

extern void* g_unk_716e34c3d0;

// One coordinate (k) of the four corners: the particle's position moved along the two quad axes.
inline void SetCoord_l1708fc(UnkQuad_l1708fc* quad, int k, const f32* pos, f32 right, f32 down,
                             const CVector2D* corners) {
    for (int j = 0; j < 4; j++) {
        quad->v[j].pos[k] = pos[k] + right * corners[j].x + down * corners[j].y;
    }
}

// Texture coordinates of the corner (x, y) of the unit square.
inline void SetUV_l1708fc(UnkVertex_l1708fc* vertex, const UnkUVTransform_l1708fc& t, f32 x, f32 y) {
    vertex->uv[0] = t.xx * x - t.xy * y - t.tx;
    vertex->uv[1] = t.yx * x + t.yy * y - t.ty;
}

void fn_71001708fc(UnkEmitter_l1708fc* self, const UnkVec3f& dir) {
    UnkQuad_l1708fc* quad = (UnkQuad_l1708fc*)g_unk_716e34c3d0;
    CVector3D side;
    side.x = 1.0f;
    side.y = 0.0f;
    side.z = 0.0f;
    CVector3D fwd;
    fwd.x = 0.0f;
    fwd.y = 1.0f;
    fwd.z = 0.0f;
    UnkVec3f up = dir;
    if (!self->field_0x43a) {
        up = fn_7100077c04(self->field_0x3f0, up);
        f32 inv = 1.0f / (sqrtf(up.x * up.x + up.y * up.y + up.z * up.z) + 0.0001f);
        up.x *= inv;
        up.y *= inv;
        up.z *= inv;
    }

    u32 count = self->count;
    if (count == 0) {
        return;
    }
    s32 flags = self->sizeFlags;
    CVector2D scale;
    scale.x = self->scaleX;
    scale.y = self->scaleY;
    CVector2D addScale = scale * (f32)(flags & 2);
    CVector2D mulScale = scale * (f32)(flags & 1);
    for (u32 i = 0; i < count; i++, quad++) {
        const u32& index = self->indices[i];
        const UnkColor_l1708fc& color = self->color.Get(index);
        const u32& alpha = self->alpha.Get(index);
        const CVector2D& pivot = self->uvPivot.Get(index);
        const CVector2D& uvScale = self->uvScale.Get(index);
        const CVector3D& a = self->sizeA.Get(index);
        const CVector3D& b = self->sizeB.Get(index);
        f32 width = addScale.x * (a.x + b.x) + mulScale.x * (a.x * b.x);
        f32 height = addScale.y * (a.y + b.y) + mulScale.y * (a.y * b.y);
        const CVector3D& pos = self->positions[index];
        fn_710016fa84((UnkOwner_710016fa84*)self, (const CVector3D*)&up, &self->rotation.Get(index),
                      &side, &fwd);

        SetCoord_l1708fc(quad, 0, &pos.x, width * fwd.x, height * side.x, self->corners);
        SetCoord_l1708fc(quad, 1, &pos.x, width * fwd.y, height * side.y, self->corners);
        SetCoord_l1708fc(quad, 2, &pos.x, width * fwd.z, height * side.z, self->corners);
        quad->v[0].color[0] = quad->v[1].color[0] = quad->v[2].color[0] = quad->v[3].color[0] = color.rgb[0];
        quad->v[0].color[1] = quad->v[1].color[1] = quad->v[2].color[1] = quad->v[3].color[1] = color.rgb[1];
        quad->v[0].color[2] = quad->v[1].color[2] = quad->v[2].color[2] = quad->v[3].color[2] = color.rgb[2];
        quad->v[0].color[3] = quad->v[1].color[3] = quad->v[2].color[3] = quad->v[3].color[3] = alpha;

        f32 angle = self->angle.Get(index);
        f32 s = -sinf(angle);
        f32 c = cosf(angle);
        UnkUVTransform_l1708fc t;
        t.xx = c * uvScale.x;
        t.xy = uvScale.x * s;
        t.tx = uvScale.x * pivot.x - 0.5f;
        t.yx = uvScale.y * s;
        t.yy = c * uvScale.y;
        t.ty = uvScale.y * pivot.y - 0.5f;
        SetUV_l1708fc(&quad->v[0], t, 0.5f, -0.5f);
        SetUV_l1708fc(&quad->v[1], t, -0.5f, -0.5f);
        SetUV_l1708fc(&quad->v[2], t, 0.5f, 0.5f);
        SetUV_l1708fc(&quad->v[3], t, -0.5f, 0.5f);
    }
}
