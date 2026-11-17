// A unit with no known name (unit 210): writes one camera-facing quad per live particle of an emitter
// into the frame's vertex buffer, for emitters with a per-particle rotation, size, colour, alpha and texture
// pivot, and one texture angle and scale. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/math/CVectorOps.h"
#include "game/UnkTU54.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// A per-particle value, or one value for every particle when there is no array.
struct UnkColor_l17c9fc {
    u32 rgb[3];
};

template <class T>
struct UnkPerParticle_l17c9fc {
    T value;
    T* values;
    const T& Get(u32 i) const { return values ? values[i] : value; }
};

struct UnkEmitter_l17c9fc {
    u8 pad_0x0[0x88];
    CVector3D* positions;  // 0x88
    u8 pad_0x90[0x10];
    UnkPerParticle_l17c9fc<CVector3D> rotation;  // 0xa0
    u8 pad_0xb8[0x10];
    UnkPerParticle_l17c9fc<CVector3D> sizeA;  // 0xc8
    u8 pad_0xe0[0x10];
    UnkPerParticle_l17c9fc<CVector3D> sizeB;  // 0xf0
    u8 pad_0x108[0x38];
    UnkPerParticle_l17c9fc<UnkColor_l17c9fc> color;  // 0x140
    u8 pad_0x158[0x10];
    UnkPerParticle_l17c9fc<u32> alpha;  // 0x168
    u8 pad_0x178[0x10];
    UnkPerParticle_l17c9fc<CVector2D> uvPivot;  // 0x188
    u8 pad_0x198[0x10];
    f32 angle;  // 0x1a8
    u8 pad_0x1ac[0x1c];
    CVector2D uvScale;  // 0x1c8
    u8 pad_0x1d0[0x178];
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
static_assert(__builtin_offsetof(UnkEmitter_l17c9fc, rotation) == 0xa0, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l17c9fc, sizeA) == 0xc8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l17c9fc, sizeB) == 0xf0, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l17c9fc, color) == 0x140, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l17c9fc, alpha) == 0x168, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l17c9fc, uvPivot) == 0x188, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l17c9fc, angle) == 0x1a8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l17c9fc, uvScale) == 0x1c8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l17c9fc, sizeFlags) == 0x348, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l17c9fc, count) == 0x390, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l17c9fc, field_0x3f0) == 0x3f0, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l17c9fc, field_0x43a) == 0x43a, "layout");

struct UnkVertex_l17c9fc {
    f32 pos[3];
    f32 uv[2];
    u32 color[4];
};

struct UnkQuad_l17c9fc {
    UnkVertex_l17c9fc v[4];
};

struct UnkOwner_710016fa84;
void fn_710016fa84(UnkOwner_710016fa84* self, const CVector3D* up, const CVector3D* rot,
                   CVector3D* side, CVector3D* fwd);

extern void* g_unk_716e34c3d0;

// One corner's position: the particle's position moved along the two quad axes by the corner offset.
inline void SetPos_l17c9fc(UnkVertex_l17c9fc* vertex, const CVector3D* pos, const CVector3D& right,
                           const CVector3D& down, const CVector2D& corner) {
    vertex->pos[0] = pos->x + right.x * corner.x + down.x * corner.y;
    vertex->pos[1] = pos->y + right.y * corner.x + down.y * corner.y;
    vertex->pos[2] = pos->z + right.z * corner.x + down.z * corner.y;
}

// The texture offset of one corner: the corner's x and y half-extents, each already turned by the angle
// (sx = sin * x, cx = cos * x, and so on).
inline CVector2D UVOffset_l17c9fc(f32 cx, f32 sy, f32 sx, f32 cy) {
    CVector2D uv;
    uv.x = cx - sy;
    uv.y = sx + cy;
    return uv;
}

// Texture coordinates of one corner: its offset from the pivot, scaled about the centre of the texture.
inline void SetUV_l17c9fc(UnkVertex_l17c9fc* vertex, const CVector2D& offset, const CVector2D* pivot,
                          const CVector2D* scale) {
    vertex->uv[0] = (offset.x - pivot->x) * scale->x + 0.5f;
    vertex->uv[1] = (offset.y - pivot->y) * scale->y + 0.5f;
}

void fn_710017c9fc(UnkEmitter_l17c9fc* self, const UnkVec3f& dir) {
    UnkQuad_l17c9fc* quads = (UnkQuad_l17c9fc*)g_unk_716e34c3d0;
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
        up = fn_7100077c04(self->field_0x3f0, up);
        f32 inv = 1.0f / (sqrtf(up.x * up.x + up.y * up.y + up.z * up.z) + 0.0001f);
        up.x *= inv;
        up.y *= inv;
        up.z *= inv;
    }
    f32 angle = self->angle;
    f32 s = sinf(angle);
    f32 c = cosf(angle);
    f32 sPos = s * 0.5f;
    f32 sNeg = s * -0.5f;
    f32 cPos = c * 0.5f;
    f32 cNeg = c * -0.5f;

    u32 count = self->count;
    if (count == 0) {
        return;
    }
    CVector2D addScale = scale * (f32)(flags & 2);
    CVector2D mulScale = scale * (f32)(flags & 1);
    for (u32 i = 0; i < count; i++) {
        UnkQuad_l17c9fc* quad = &quads[i];
        const u32* indices = self->indices;
        u32 index = indices[(s32)i];
        const CVector3D& a = self->sizeA.Get(index);
        const CVector3D& b = self->sizeB.Get(index);
        f32 width = addScale.x * (a.x + b.x) + mulScale.x * (a.x * b.x);
        f32 height = addScale.y * (a.y + b.y) + mulScale.y * (a.y * b.y);
        const CVector3D* pos = &self->positions[index];
        fn_710016fa84((UnkOwner_710016fa84*)self, (const CVector3D*)&up, &self->rotation.Get(index), &side,
                      &fwd);
        CVector3D right = fwd * width;
        CVector3D down = side * height;
        SetPos_l17c9fc(&quad->v[0], pos, right, down, self->corners[0]);
        SetPos_l17c9fc(&quad->v[1], pos, right, down, self->corners[1]);
        SetPos_l17c9fc(&quad->v[2], pos, right, down, self->corners[2]);
        SetPos_l17c9fc(&quad->v[3], pos, right, down, self->corners[3]);
        u32 colorIndex = indices[(s32)i];
        const UnkColor_l17c9fc& color = self->color.Get(colorIndex);
        const u32& alpha = self->alpha.Get(colorIndex);
        quad->v[0].color[0] = quad->v[1].color[0] = quad->v[2].color[0] = quad->v[3].color[0] = color.rgb[0];
        quad->v[0].color[1] = quad->v[1].color[1] = quad->v[2].color[1] = quad->v[3].color[1] = color.rgb[1];
        quad->v[0].color[2] = quad->v[1].color[2] = quad->v[2].color[2] = quad->v[3].color[2] = color.rgb[2];
        quad->v[0].color[3] = quad->v[1].color[3] = quad->v[2].color[3] = quad->v[3].color[3] = alpha;

        u32 uvIndex = indices[(s32)i];
        const CVector2D* pivot = &self->uvPivot.Get(uvIndex);
        const CVector2D* uvScale = &self->uvScale;
        SetUV_l17c9fc(&quad->v[0], UVOffset_l17c9fc(cPos, sPos, sNeg, cNeg), pivot, uvScale);
        SetUV_l17c9fc(&quad->v[1], UVOffset_l17c9fc(cNeg, sPos, sPos, cNeg), pivot, uvScale);
        SetUV_l17c9fc(&quad->v[2], UVOffset_l17c9fc(cPos, sNeg, sNeg, cPos), pivot, uvScale);
        SetUV_l17c9fc(&quad->v[3], UVOffset_l17c9fc(cNeg, sNeg, sPos, cPos), pivot, uvScale);
    }
}
