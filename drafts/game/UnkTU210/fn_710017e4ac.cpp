// A unit with no known name (unit 210): writes one camera-facing quad per live particle of an emitter
// into the frame's vertex buffer, for emitters with one texture angle, a per-particle colour and a
// per-particle texture pivot. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/math/CVectorOps.h"
#include "game/UnkTU54.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// A per-particle value, or one value for every particle when there is no array.
template <class T>
struct UnkPerParticle_l17e4ac {
    T value;
    T* values;
    const T& Get(u32 i) const { return values ? values[i] : value; }
};

struct UnkColor_l17e4ac {
    u32 rgb[3];
};

struct UnkEmitter_l17e4ac {
    u8 pad_0x0[0x88];
    CVector3D* positions;  // 0x88
    u8 pad_0x90[0x10];
    CVector3D rotation;  // 0xa0
    u8 pad_0xac[0x1c];
    CVector2D sizeA;  // 0xc8
    u8 pad_0xd0[0x20];
    CVector2D sizeB;  // 0xf0
    u8 pad_0xf8[0x48];
    UnkPerParticle_l17e4ac<UnkColor_l17e4ac> color;  // 0x140
    u8 pad_0x158[0x10];
    u32 alpha;  // 0x168
    u8 pad_0x16c[0x1c];
    UnkPerParticle_l17e4ac<CVector2D> uvPivot;  // 0x188
    u8 pad_0x198[0x10];
    UnkPerParticle_l17e4ac<f32> angle;  // 0x1a8
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
static_assert(__builtin_offsetof(UnkEmitter_l17e4ac, color) == 0x140, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l17e4ac, alpha) == 0x168, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l17e4ac, uvPivot) == 0x188, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l17e4ac, angle) == 0x1a8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l17e4ac, uvScale) == 0x1c8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l17e4ac, sizeFlags) == 0x348, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l17e4ac, count) == 0x390, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l17e4ac, field_0x3f0) == 0x3f0, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l17e4ac, field_0x43a) == 0x43a, "layout");

struct UnkVertex_l17e4ac {
    CVector3D pos;
    CVector2D uv;
    u32 color[4];
};

struct UnkQuad_l17e4ac {
    UnkVertex_l17e4ac v[4];
};

struct UnkOwner_710016fa84;
void fn_710016fa84(UnkOwner_710016fa84* self, const CVector3D* up, const CVector3D* rot,
                   CVector3D* side, CVector3D* fwd);

extern void* g_unk_716e34c3d0;

// One corner's position: the particle's position moved by the corner offset.
inline void SetPos_l17e4ac(UnkVertex_l17e4ac* vertex, const CVector3D& corner, const CVector3D* pos) {
    vertex->pos.x = corner.x + pos->x;
    vertex->pos.y = corner.y + pos->y;
    vertex->pos.z = corner.z + pos->z;
}

// The texture offset of one corner (x, y) of the unit square, turned by the angle.
inline CVector2D UVOffset_l17e4ac(f32 s, f32 c, f32 x, f32 y) {
    CVector2D uv;
    uv.x = c * x - s * y;
    uv.y = -s * x + -c * y;
    return uv;
}

// Texture coordinates of one corner: its offset from the pivot, scaled about the centre of the texture.
inline void SetUV_l17e4ac(UnkVertex_l17e4ac* vertex, const CVector2D& offset, const CVector2D* pivot,
                          const CVector2D* scale) {
    vertex->uv.x = (offset.x - pivot->x) * scale->x + 0.5f;
    vertex->uv.y = (offset.y - pivot->y) * scale->y + 0.5f;
}

void fn_710017e4ac(UnkEmitter_l17e4ac* self, const UnkVec3f& dir) {
    UnkQuad_l17e4ac* quad = (UnkQuad_l17e4ac*)g_unk_716e34c3d0;
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

    f32 angle = self->angle.value;
    f32 add = (f32)(flags & 2);
    f32 mul = (f32)(flags & 1);
    f32 width = scaleX * add * (self->sizeA.x + self->sizeB.x) +
                scaleX * mul * (self->sizeA.x * self->sizeB.x);
    f32 height = scaleY * add * (self->sizeA.y + self->sizeB.y) +
                 scaleY * mul * (self->sizeA.y * self->sizeB.y);
    CVector3D right = fwd * width;
    CVector3D down = side * height;
    CVector3D corner0 = right * self->corners[0].x + down * self->corners[0].y;
    CVector3D corner1 = right * self->corners[1].x + down * self->corners[1].y;
    CVector3D corner2 = right * self->corners[2].x + down * self->corners[2].y;
    CVector3D corner3 = right * self->corners[3].x + down * self->corners[3].y;
    f32 s = sinf(angle);
    f32 c = cosf(angle);
    CVector2D uv0 = UVOffset_l17e4ac(s, c, 0.5f, 0.5f);
    CVector2D uv1 = UVOffset_l17e4ac(s, c, -0.5f, 0.5f);
    CVector2D uv2 = UVOffset_l17e4ac(s, c, 0.5f, -0.5f);
    CVector2D uv3 = UVOffset_l17e4ac(s, c, -0.5f, -0.5f);

    u32 count = self->count;
    for (u32 i = 0; i < count; i++, quad++) {
        const CVector3D* pos = &self->positions[self->indices[i]];
        SetPos_l17e4ac(&quad->v[0], corner0, pos);
        SetPos_l17e4ac(&quad->v[1], corner1, pos);
        SetPos_l17e4ac(&quad->v[2], corner2, pos);
        SetPos_l17e4ac(&quad->v[3], corner3, pos);
        const UnkColor_l17e4ac* color = &self->color.Get(self->indices[i]);
        quad->v[0].color[0] = quad->v[1].color[0] = quad->v[2].color[0] = quad->v[3].color[0] = color->rgb[0];
        quad->v[0].color[1] = quad->v[1].color[1] = quad->v[2].color[1] = quad->v[3].color[1] = color->rgb[1];
        quad->v[0].color[2] = quad->v[1].color[2] = quad->v[2].color[2] = quad->v[3].color[2] = color->rgb[2];
        quad->v[0].color[3] = quad->v[1].color[3] = quad->v[2].color[3] = quad->v[3].color[3] = self->alpha;

        const CVector2D* pivot = &self->uvPivot.Get(self->indices[i]);
        SetUV_l17e4ac(&quad->v[0], uv0, pivot, &self->uvScale);
        SetUV_l17e4ac(&quad->v[1], uv1, pivot, &self->uvScale);
        SetUV_l17e4ac(&quad->v[2], uv2, pivot, &self->uvScale);
        SetUV_l17e4ac(&quad->v[3], uv3, pivot, &self->uvScale);
    }
}
