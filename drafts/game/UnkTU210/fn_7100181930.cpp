// A unit with no known name (unit 210): writes one camera-facing quad per live particle of an emitter
// into the frame's vertex buffer, for emitters with a per-particle size, colour, per-particle
// orientation, texture angle and texture scale, with one alpha and one pivot. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/math/CVectorOps.h"
#include "game/UnkTU54.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// A per-particle value, or one value for every particle when there is no array.
template <class T>
struct UnkPerParticle_l181930 {
    T value;
    T* values;
    const T& Get(u32 i) const { return values ? values[i] : value; }
};

struct UnkColor_l181930 {
    u32 rgb[3];
};

struct UnkEmitter_l181930 {
    u8 pad_0x0[0x88];
    CVector3D* positions;  // 0x88
    u8 pad_0x90[0x10];
    UnkPerParticle_l181930<CVector3D> rotation;  // 0xa0
    u8 pad_0xb8[0x10];
    UnkPerParticle_l181930<CVector3D> sizeA;  // 0xc8
    u8 pad_0xe0[0x10];
    UnkPerParticle_l181930<CVector3D> sizeB;  // 0xf0
    u8 pad_0x108[0x38];
    UnkPerParticle_l181930<UnkColor_l181930> color;  // 0x140
    u8 pad_0x158[0x10];
    u32 alpha;  // 0x168
    u8 pad_0x16c[0x1c];
    UnkPerParticle_l181930<CVector2D> uvPivot;  // 0x188
    u8 pad_0x198[0x10];
    UnkPerParticle_l181930<f32> angle;  // 0x1a8
    u8 pad_0x1b8[0x10];
    UnkPerParticle_l181930<CVector2D> uvScale;  // 0x1c8
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
static_assert(__builtin_offsetof(UnkEmitter_l181930, sizeA) == 0xc8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l181930, sizeB) == 0xf0, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l181930, color) == 0x140, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l181930, alpha) == 0x168, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l181930, uvPivot) == 0x188, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l181930, angle) == 0x1a8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l181930, uvScale) == 0x1c8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l181930, sizeFlags) == 0x348, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l181930, count) == 0x390, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l181930, field_0x3f0) == 0x3f0, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l181930, field_0x43a) == 0x43a, "layout");

struct UnkVertex_l181930 {
    f32 pos[3];
    f32 uv[2];
    u32 color[4];
};

struct UnkQuad_l181930 {
    UnkVertex_l181930 v[4];
};

struct UnkOwner_710016fa84;
void fn_710016fa84(UnkOwner_710016fa84* self, const CVector3D* up, const CVector3D* rot,
                   CVector3D* side, CVector3D* fwd);

extern void* g_unk_716e34c3d0;

// One corner's position: the particle's position moved along both quad axes by the corner offset.
inline void SetPos_l181930(UnkVertex_l181930* vertex, const CVector3D* pos, const CVector3D& right,
                           const CVector3D& down, const CVector2D& corner) {
    vertex->pos[0] = pos->x + right.x * corner.x + down.x * corner.y;
    vertex->pos[1] = pos->y + right.y * corner.x + down.y * corner.y;
    vertex->pos[2] = pos->z + right.z * corner.x + down.z * corner.y;
}

// Texture coordinates of one corner: its offset from the pivot, scaled about the centre of the texture.
inline void SetUV_l181930(UnkVertex_l181930* vertex, const CVector2D& offset, const CVector2D* pivot,
                          const CVector2D* scale) {
    vertex->uv[0] = (offset.x - pivot->x) * scale->x + 0.5f;
    vertex->uv[1] = (offset.y - pivot->y) * scale->y + 0.5f;
}

void fn_7100181930(UnkEmitter_l181930* self, const UnkVec3f& dir) {
    UnkQuad_l181930* quads = (UnkQuad_l181930*)g_unk_716e34c3d0;
    CVector3D side;
    side.x = 0.0f;
    side.y = 0.0f;
    side.z = 0.0f;
    CVector3D fwd;
    fwd.x = 0.0f;
    fwd.y = 0.0f;
    fwd.z = 0.0f;
    s32 flags = self->sizeFlags;
    f32 scaleX = self->scaleX;
    f32 scaleY = self->scaleY;
    UnkVec3f up = dir;
    if (!self->field_0x43a) {
        up = fn_7100077c04(self->field_0x3f0, up);
        f32 inv = 1.0f / (sqrtf(up.x * up.x + up.y * up.y + up.z * up.z) + 0.0001f);
        up.x *= inv;
        up.y *= inv;
        up.z *= inv;
    }

    f32 add = (f32)(flags & 2);
    f32 mul = (f32)(flags & 1);
    CVector2D scale = {scaleX, scaleY};
    CVector2D addScale = scale * add;
    CVector2D mulScale = scale * mul;
    s32 count = self->count;
    for (s32 i = 0; i < count; i++) {
        UnkQuad_l181930* quad = &quads[i];
        const u32* indices = self->indices;
        u32 index = indices[i];
        const CVector3D& sizeA = self->sizeA.Get(index);
        const CVector3D& sizeB = self->sizeB.Get(index);
        CVector2D sum = {sizeA.x + sizeB.x, sizeA.y + sizeB.y};
        CVector2D product = {sizeA.x * sizeB.x, sizeA.y * sizeB.y};
        f32 width = addScale.x * sum.x + mulScale.x * product.x;
        f32 height = addScale.y * sum.y + mulScale.y * product.y;
        const CVector3D* rotation = &self->rotation.Get(index);
        const CVector3D* pos = &self->positions[index];
        fn_710016fa84((UnkOwner_710016fa84*)self, (const CVector3D*)&up, rotation, &side, &fwd);
        CVector3D right = fwd * width;
        CVector3D down = side * height;
        SetPos_l181930(&quad->v[0], pos, right, down, self->corners[0]);
        SetPos_l181930(&quad->v[1], pos, right, down, self->corners[1]);
        SetPos_l181930(&quad->v[2], pos, right, down, self->corners[2]);
        SetPos_l181930(&quad->v[3], pos, right, down, self->corners[3]);

        u32 colorIndex = indices[i];
        const UnkColor_l181930* color = &self->color.Get(colorIndex);
        quad->v[0].color[0] = quad->v[1].color[0] = quad->v[2].color[0] = quad->v[3].color[0] = color->rgb[0];
        quad->v[0].color[1] = quad->v[1].color[1] = quad->v[2].color[1] = quad->v[3].color[1] = color->rgb[1];
        quad->v[0].color[2] = quad->v[1].color[2] = quad->v[2].color[2] = quad->v[3].color[2] = color->rgb[2];
        quad->v[0].color[3] = quad->v[1].color[3] = quad->v[2].color[3] = quad->v[3].color[3] = self->alpha;

        u32 uvIndex = indices[i];
        const CVector2D* uvScale = &self->uvScale.Get(uvIndex);
        f32 angle = self->angle.Get(uvIndex);
        f32 s = sinf(angle);
        f32 c = cosf(angle);
        const CVector2D* pivot = &self->uvPivot.value;
        // The four corners of the unit square around its centre, turned by the angle.
        f32 halfSin = s * 0.5f;
        f32 negHalfSin = s * -0.5f;
        f32 halfCos = c * 0.5f;
        f32 negHalfCos = c * -0.5f;
        CVector2D offset0 = {halfCos - halfSin, negHalfSin + negHalfCos};
        CVector2D offset1 = {negHalfCos - halfSin, halfSin + negHalfCos};
        CVector2D offset2 = {halfCos - negHalfSin, negHalfSin + halfCos};
        CVector2D offset3 = {negHalfCos - negHalfSin, halfSin + halfCos};
        SetUV_l181930(&quad->v[0], offset0, pivot, uvScale);
        SetUV_l181930(&quad->v[1], offset1, pivot, uvScale);
        SetUV_l181930(&quad->v[2], offset2, pivot, uvScale);
        SetUV_l181930(&quad->v[3], offset3, pivot, uvScale);
    }
}
