// A unit with no known name (unit 210): writes one camera-facing quad per live particle of an emitter
// into the frame's vertex buffer, for emitters with one size and texture scale and a per-particle colour,
// alpha, texture pivot and angle. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/math/CVectorOps.h"
#include "game/UnkTU54.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// A per-particle value, or one value for every particle when there is no array.
template <class T>
struct UnkPerParticle_l175520 {
    T value;
    T* values;
    const T& Get(u32 i) const { return values ? values[i] : value; }
};

struct UnkColor_l175520 {
    u32 rgb[3];
};

struct UnkEmitter_l175520 {
    u8 pad_0x0[0x88];
    CVector3D* positions;  // 0x88
    u8 pad_0x90[0x10];
    CVector3D rotation;  // 0xa0
    u8 pad_0xac[0x1c];
    UnkPerParticle_l175520<CVector3D> sizeA;  // 0xc8
    u8 pad_0xe0[0x10];
    UnkPerParticle_l175520<CVector3D> sizeB;  // 0xf0
    u8 pad_0x108[0x38];
    UnkPerParticle_l175520<UnkColor_l175520> color;  // 0x140
    u8 pad_0x158[0x10];
    UnkPerParticle_l175520<u32> alpha;  // 0x168
    u8 pad_0x178[0x10];
    UnkPerParticle_l175520<CVector2D> uvPivot;  // 0x188
    u8 pad_0x198[0x10];
    UnkPerParticle_l175520<f32> angle;  // 0x1a8
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
static_assert(__builtin_offsetof(UnkEmitter_l175520, sizeA) == 0xc8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l175520, sizeB) == 0xf0, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l175520, color) == 0x140, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l175520, alpha) == 0x168, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l175520, uvPivot) == 0x188, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l175520, angle) == 0x1a8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l175520, uvScale) == 0x1c8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l175520, sizeFlags) == 0x348, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l175520, count) == 0x390, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l175520, field_0x3f0) == 0x3f0, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l175520, field_0x43a) == 0x43a, "layout");

struct UnkVertex_l175520 {
    CVector3D pos;
    CVector2D uv;
    u32 color[4];
};

struct UnkQuad_l175520 {
    UnkVertex_l175520 v[4];
};

struct UnkOwner_710016fa84;
void fn_710016fa84(UnkOwner_710016fa84* self, const CVector3D* up, const CVector3D* rot,
                   CVector3D* side, CVector3D* fwd);

extern void* g_unk_716e34c3d0;

// One corner's position: the particle's position moved by the corner offset.
inline void SetPos_l175520(UnkVertex_l175520* vertex, const CVector3D& corner, const CVector3D* pos) {
    vertex->pos.x = corner.x + pos->x;
    vertex->pos.y = corner.y + pos->y;
    vertex->pos.z = corner.z + pos->z;
}

// The texture offset of one corner (x, y) of the unit square, turned by the angle.
inline CVector2D UVOffset_l175520(f32 s, f32 c, f32 x, f32 y) {
    CVector2D uv;
    uv.x = c * x - s * y;
    uv.y = -s * x + -c * y;
    return uv;
}

// Texture coordinates of one corner: its offset from the pivot, scaled about the centre of the texture.
inline void SetUV_l175520(UnkVertex_l175520* vertex, const CVector2D& offset, const CVector2D* pivot,
                          const CVector2D* scale) {
    vertex->uv.x = (offset.x - pivot->x) * scale->x + 0.5f;
    vertex->uv.y = (offset.y - pivot->y) * scale->y + 0.5f;
}

void fn_7100175520(UnkEmitter_l175520* self, const UnkVec3f& dir) {
    UnkQuad_l175520* quad = (UnkQuad_l175520*)g_unk_716e34c3d0;
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
        up = TransformDirectionByMatrix3x3(self->field_0x3f0, up);
        f32 inv = 1.0f / (sqrtf(up.x * up.x + up.y * up.y + up.z * up.z) + 0.0001f);
        up.x *= inv;
        up.y *= inv;
        up.z *= inv;
    }
    f32 scaleX = self->scaleX;
    f32 scaleY = self->scaleY;
    s32 flags = self->sizeFlags;
    fn_710016fa84((UnkOwner_710016fa84*)self, (const CVector3D*)&up, &self->rotation, &side, &fwd);

    u32 count = self->count;
    if (count == 0) {
        return;
    }
    f32 add = (f32)(flags & 2);
    f32 mul = (f32)(flags & 1);
    const CVector3D& a = self->sizeA.value;
    const CVector3D& b = self->sizeB.value;
    f32 width = scaleX * add * (a.x + b.x) + scaleX * mul * (a.x * b.x);
    f32 height = scaleY * add * (a.y + b.y) + scaleY * mul * (a.y * b.y);
    CVector3D right = fwd * width;
    CVector3D down = side * height;
    CVector3D corner0 = right * self->corners[0].x + down * self->corners[0].y;
    CVector3D corner1 = right * self->corners[1].x + down * self->corners[1].y;
    CVector3D corner2 = right * self->corners[2].x + down * self->corners[2].y;
    CVector3D corner3 = right * self->corners[3].x + down * self->corners[3].y;

    for (u32 i = 0; i < count; i++, quad++) {
        const u32& index = self->indices[i];
        const CVector3D* pos = &self->positions[index];
        SetPos_l175520(&quad->v[0], corner0, pos);
        SetPos_l175520(&quad->v[1], corner1, pos);
        SetPos_l175520(&quad->v[2], corner2, pos);
        SetPos_l175520(&quad->v[3], corner3, pos);
        const UnkColor_l175520* color = &self->color.Get(index);
        const u32* alpha = &self->alpha.Get(index);
        quad->v[0].color[0] = quad->v[1].color[0] = quad->v[2].color[0] = quad->v[3].color[0] = color->rgb[0];
        quad->v[0].color[1] = quad->v[1].color[1] = quad->v[2].color[1] = quad->v[3].color[1] = color->rgb[1];
        quad->v[0].color[2] = quad->v[1].color[2] = quad->v[2].color[2] = quad->v[3].color[2] = color->rgb[2];
        quad->v[0].color[3] = quad->v[1].color[3] = quad->v[2].color[3] = quad->v[3].color[3] = *alpha;

        const CVector2D* pivot = &self->uvPivot.Get(index);
        f32 angle = self->angle.Get(index);
        f32 s = sinf(angle);
        f32 c = cosf(angle);
        SetUV_l175520(&quad->v[0], UVOffset_l175520(s, c, 0.5f, 0.5f), pivot, &self->uvScale);
        SetUV_l175520(&quad->v[1], UVOffset_l175520(s, c, -0.5f, 0.5f), pivot, &self->uvScale);
        SetUV_l175520(&quad->v[2], UVOffset_l175520(s, c, 0.5f, -0.5f), pivot, &self->uvScale);
        SetUV_l175520(&quad->v[3], UVOffset_l175520(s, c, -0.5f, -0.5f), pivot, &self->uvScale);
    }
}
