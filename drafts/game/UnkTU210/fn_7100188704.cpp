// A unit with no known name (unit 210): writes one quad per live particle of an emitter into the frame's
// vertex buffer, each particle with its own rotation, size, colour, alpha and texture scale, and one
// texture angle and pivot for the whole emitter. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/math/CVectorOps.h"
#include "game/UnkTU54.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// A per-particle value, or one value for every particle when there is no array.
template <class T>
struct UnkPerParticle_l188704 {
    T value;
    T* values;
    const T& Get(u32 i) const { return values ? values[i] : value; }
};

struct UnkColor_l188704 {
    u32 rgb[3];
};

struct UnkEmitter_l188704 {
    u8 pad_0x0[0x88];
    CVector3D* positions;  // 0x88
    u8 pad_0x90[0x10];
    UnkPerParticle_l188704<CVector3D> rotation;  // 0xa0
    u8 pad_0xb8[0x10];
    UnkPerParticle_l188704<CVector3D> sizeA;  // 0xc8
    u8 pad_0xe0[0x10];
    UnkPerParticle_l188704<CVector3D> sizeB;  // 0xf0
    u8 pad_0x108[0x38];
    UnkPerParticle_l188704<UnkColor_l188704> color;  // 0x140
    u8 pad_0x158[0x10];
    UnkPerParticle_l188704<u32> alpha;  // 0x168
    u8 pad_0x178[0x10];
    CVector2D uvPivot;  // 0x188
    u8 pad_0x190[0x18];
    f32 angle;  // 0x1a8
    u8 pad_0x1ac[0x1c];
    UnkPerParticle_l188704<CVector2D> uvScale;  // 0x1c8
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
static_assert(__builtin_offsetof(UnkEmitter_l188704, rotation) == 0xa0, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l188704, sizeA) == 0xc8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l188704, sizeB) == 0xf0, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l188704, color) == 0x140, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l188704, alpha) == 0x168, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l188704, uvPivot) == 0x188, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l188704, angle) == 0x1a8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l188704, uvScale) == 0x1c8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l188704, sizeFlags) == 0x348, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l188704, count) == 0x390, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l188704, field_0x3f0) == 0x3f0, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l188704, field_0x43a) == 0x43a, "layout");

struct UnkVertex_l188704 {
    f32 pos[3];
    f32 uv[2];
    u32 color[4];
};

struct UnkQuad_l188704 {
    UnkVertex_l188704 v[4];
};

struct UnkOwner_710016fa84;
void fn_710016fa84(UnkOwner_710016fa84* self, const CVector3D* up, const CVector3D* rot,
                   CVector3D* side, CVector3D* fwd);

extern void* g_unk_716e34c3d0;

// One corner's position: the particle's position moved along the two quad axes by the corner offset.
inline void SetPos_l188704(UnkVertex_l188704* vertex, const CVector3D* pos, const CVector3D& right,
                           const CVector3D& down, const CVector2D& corner) {
    vertex->pos[0] = pos->x + right.x * corner.x + down.x * corner.y;
    vertex->pos[1] = pos->y + right.y * corner.x + down.y * corner.y;
    vertex->pos[2] = pos->z + right.z * corner.x + down.z * corner.y;
}

// A texture offset relative to the pivot.
inline CVector2D UVOffset_l188704(f32 x, f32 y, const CVector2D& pivot) {
    CVector2D uv;
    uv.x = x - pivot.x;
    uv.y = y - pivot.y;
    return uv;
}

// Texture coordinates of one corner: its offset from the pivot, scaled about the centre of the texture.
inline void SetUV_l188704(UnkVertex_l188704* vertex, const CVector2D& offset, const CVector2D* scale) {
    vertex->uv[0] = offset.x * scale->x + 0.5f;
    vertex->uv[1] = offset.y * scale->y + 0.5f;
}

void fn_7100188704(UnkEmitter_l188704* self, const UnkVec3f& dir) {
    UnkQuad_l188704* quad = (UnkQuad_l188704*)g_unk_716e34c3d0;
    CVector3D side;
    side.x = 0.0f;
    side.y = 0.0f;
    side.z = 0.0f;
    CVector3D fwd;
    fwd.x = 0.0f;
    fwd.y = 0.0f;
    fwd.z = 0.0f;
    s32 flags = self->sizeFlags;
    CVector2D scale;
    scale.x = self->scaleX;
    scale.y = self->scaleY;
    UnkVec3f up = dir;
    if (!self->field_0x43a) {
        up = TransformDirectionByMatrix3x3(self->field_0x3f0, up);
        f32 inv = 1.0f / (sqrtf(up.x * up.x + up.y * up.y + up.z * up.z) + 0.0001f);
        up.x *= inv;
        up.y *= inv;
        up.z *= inv;
    }
    f32 angle = self->angle;
    f32 s = sinf(angle);
    f32 c = cosf(angle);

    u32 count = self->count;
    if (count == 0) {
        return;
    }
    CVector2D addScale = scale * (f32)(flags & 2);
    CVector2D mulScale = scale * (f32)(flags & 1);
    // The corners of the unit square turned by the angle, from these four half-extent products.
    f32 cosHalf = c * 0.5f;
    f32 sinHalf = s * 0.5f;
    f32 sinNegHalf = s * -0.5f;
    f32 cosNegHalf = c * -0.5f;
    const CVector2D& pivot = self->uvPivot;
    CVector2D uv0 = UVOffset_l188704(cosHalf - sinHalf, sinNegHalf + cosNegHalf, pivot);
    CVector2D uv1 = UVOffset_l188704(cosNegHalf - sinHalf, sinHalf + cosNegHalf, pivot);
    CVector2D uv2 = UVOffset_l188704(cosHalf - sinNegHalf, sinNegHalf + cosHalf, pivot);
    CVector2D uv3 = UVOffset_l188704(cosNegHalf - sinNegHalf, sinHalf + cosHalf, pivot);
    for (u32 i = 0; i < count; i++, quad++) {
        const CVector3D& a = self->sizeA.Get(self->indices[i]);
        const CVector3D& b = self->sizeB.Get(self->indices[i]);
        f32 width = addScale.x * (a.x + b.x) + mulScale.x * (a.x * b.x);
        f32 height = addScale.y * (a.y + b.y) + mulScale.y * (a.y * b.y);
        const CVector3D* pos = &self->positions[self->indices[i]];
        fn_710016fa84((UnkOwner_710016fa84*)self, (const CVector3D*)&up, &self->rotation.Get(self->indices[i]),
                      &side, &fwd);
        CVector3D right = fwd * width;
        CVector3D down = side * height;
        SetPos_l188704(&quad->v[0], pos, right, down, self->corners[0]);
        SetPos_l188704(&quad->v[1], pos, right, down, self->corners[1]);
        SetPos_l188704(&quad->v[2], pos, right, down, self->corners[2]);
        SetPos_l188704(&quad->v[3], pos, right, down, self->corners[3]);
        const UnkColor_l188704& color = self->color.Get(self->indices[i]);
        const u32& alpha = self->alpha.Get(self->indices[i]);
        quad->v[0].color[0] = quad->v[1].color[0] = quad->v[2].color[0] = quad->v[3].color[0] = color.rgb[0];
        quad->v[0].color[1] = quad->v[1].color[1] = quad->v[2].color[1] = quad->v[3].color[1] = color.rgb[1];
        quad->v[0].color[2] = quad->v[1].color[2] = quad->v[2].color[2] = quad->v[3].color[2] = color.rgb[2];
        quad->v[0].color[3] = quad->v[1].color[3] = quad->v[2].color[3] = quad->v[3].color[3] = alpha;

        const CVector2D* uvScale = &self->uvScale.Get(self->indices[i]);
        SetUV_l188704(&quad->v[0], uv0, uvScale);
        SetUV_l188704(&quad->v[1], uv1, uvScale);
        SetUV_l188704(&quad->v[2], uv2, uvScale);
        SetUV_l188704(&quad->v[3], uv3, uvScale);
    }
}
