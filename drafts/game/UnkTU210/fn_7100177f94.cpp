// A unit with no known name (unit 210): writes one camera-facing quad per live particle of an emitter
// into the frame's vertex buffer, for emitters with a per-particle size, angle and texture pivot and one
// colour for every particle. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/math/CVectorOps.h"
#include "game/UnkTU54.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// A per-particle value, or one value for every particle when there is no array.
template <class T>
struct UnkPerParticle_l177f94 {
    T value;
    T* values;
    const T& Get(u32 i) const { return values ? values[i] : value; }
};

struct UnkColor_l177f94 {
    u32 rgb[3];
};

struct UnkEmitter_l177f94 {
    u8 pad_0x0[0x88];
    CVector3D* positions;  // 0x88
    u8 pad_0x90[0x10];
    CVector3D rotation;  // 0xa0
    u8 pad_0xac[0x1c];
    UnkPerParticle_l177f94<CVector3D> sizeA;  // 0xc8
    u8 pad_0xe0[0x10];
    UnkPerParticle_l177f94<CVector3D> sizeB;  // 0xf0
    u8 pad_0x108[0x38];
    UnkColor_l177f94 color;  // 0x140
    u8 pad_0x14c[0x1c];
    u32 alpha;  // 0x168
    u8 pad_0x16c[0x1c];
    UnkPerParticle_l177f94<CVector2D> uvPivot;  // 0x188
    u8 pad_0x198[0x10];
    UnkPerParticle_l177f94<f32> angle;  // 0x1a8
    u8 pad_0x1b8[0x10];
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
static_assert(__builtin_offsetof(UnkEmitter_l177f94, sizeA) == 0xc8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l177f94, sizeB) == 0xf0, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l177f94, color) == 0x140, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l177f94, alpha) == 0x168, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l177f94, uvPivot) == 0x188, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l177f94, angle) == 0x1a8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l177f94, uvScale) == 0x1c8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l177f94, sizeFlags) == 0x348, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l177f94, count) == 0x390, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l177f94, field_0x3f0) == 0x3f0, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l177f94, field_0x43a) == 0x43a, "layout");

struct UnkVertex_l177f94 {
    f32 pos[3];
    f32 uv[2];
    u32 color[4];
};

struct UnkQuad_l177f94 {
    UnkVertex_l177f94 v[4];
};

struct UnkOwner_710016fa84;
void fn_710016fa84(UnkOwner_710016fa84* self, const CVector3D* up, const CVector3D* rot,
                   CVector3D* side, CVector3D* fwd);

extern void* g_unk_716e34c3d0;

// One corner's position: the particle's position moved along the two quad axes by the corner offset.
inline void SetPos_l177f94(UnkVertex_l177f94* vertex, const CVector3D* pos, const CVector3D& right,
                           const CVector3D& down, const CVector2D& corner) {
    vertex->pos[0] = pos->x + right.x * corner.x + down.x * corner.y;
    vertex->pos[1] = pos->y + right.y * corner.x + down.y * corner.y;
    vertex->pos[2] = pos->z + right.z * corner.x + down.z * corner.y;
}

// The texture offset of one corner (x, y) of the unit square, turned by the angle.
inline CVector2D UVOffset_l177f94(f32 s, f32 c, f32 x, f32 y) {
    CVector2D uv;
    uv.x = c * x - s * y;
    uv.y = -s * x + -c * y;
    return uv;
}

// Texture coordinates of one corner: its offset from the pivot, scaled about the centre of the texture.
inline void SetUV_l177f94(UnkVertex_l177f94* vertex, const CVector2D& offset, const CVector2D* pivot,
                          const UnkEmitter_l177f94* self) {
    vertex->uv[0] = (offset.x - pivot->x) * self->uvScale.x + 0.5f;
    vertex->uv[1] = (offset.y - pivot->y) * self->uvScale.y + 0.5f;
}

void fn_7100177f94(UnkEmitter_l177f94* self, const UnkVec3f& dir) {
    UnkQuad_l177f94* quads = (UnkQuad_l177f94*)g_unk_716e34c3d0;
    CVector3D side;
    side.x = 0.0f;
    side.y = 0.0f;
    side.z = 0.0f;
    CVector3D fwd;
    fwd.x = 0.0f;
    fwd.y = 0.0f;
    fwd.z = 0.0f;
    UnkVec3f up = dir;
    if (!self->field_0x43a) {
        up = fn_7100077c04(self->field_0x3f0, up);
        f32 inv = 1.0f / (sqrtf(up.x * up.x + up.y * up.y + up.z * up.z) + 0.0001f);
        up.x *= inv;
        up.y *= inv;
        up.z *= inv;
    }
    fn_710016fa84((UnkOwner_710016fa84*)self, (const CVector3D*)&up, &self->rotation, &side, &fwd);

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
    for (u32 i = 0; i < count; i++) {
        UnkQuad_l177f94* quad = &quads[i];
        const CVector3D& a = self->sizeA.Get(self->indices[i]);
        const CVector3D& b = self->sizeB.Get(self->indices[i]);
        f32 width = addScale.x * (a.x + b.x) + mulScale.x * (a.x * b.x);
        f32 height = addScale.y * (a.y + b.y) + mulScale.y * (a.y * b.y);
        CVector3D right = fwd * width;
        CVector3D down = side * height;
        const CVector3D* pos = &self->positions[self->indices[i]];
        SetPos_l177f94(&quad->v[0], pos, right, down, self->corners[0]);
        SetPos_l177f94(&quad->v[1], pos, right, down, self->corners[1]);
        SetPos_l177f94(&quad->v[2], pos, right, down, self->corners[2]);
        SetPos_l177f94(&quad->v[3], pos, right, down, self->corners[3]);
        quad->v[0].color[0] = quad->v[1].color[0] = quad->v[2].color[0] = quad->v[3].color[0] = self->color.rgb[0];
        quad->v[0].color[1] = quad->v[1].color[1] = quad->v[2].color[1] = quad->v[3].color[1] = self->color.rgb[1];
        quad->v[0].color[2] = quad->v[1].color[2] = quad->v[2].color[2] = quad->v[3].color[2] = self->color.rgb[2];
        quad->v[0].color[3] = quad->v[1].color[3] = quad->v[2].color[3] = quad->v[3].color[3] = self->alpha;

        const CVector2D* pivot = &self->uvPivot.Get(self->indices[i]);
        f32 angle = self->angle.Get(self->indices[i]);
        f32 s = sinf(angle);
        f32 c = cosf(angle);
        SetUV_l177f94(&quad->v[0], UVOffset_l177f94(s, c, 0.5f, 0.5f), pivot, self);
        SetUV_l177f94(&quad->v[1], UVOffset_l177f94(s, c, -0.5f, 0.5f), pivot, self);
        SetUV_l177f94(&quad->v[2], UVOffset_l177f94(s, c, 0.5f, -0.5f), pivot, self);
        SetUV_l177f94(&quad->v[3], UVOffset_l177f94(s, c, -0.5f, -0.5f), pivot, self);
    }
}
