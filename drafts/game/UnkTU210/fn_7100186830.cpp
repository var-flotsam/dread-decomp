// A unit with no known name (unit 210): writes one camera-facing quad per live particle of an emitter
// into the frame's vertex buffer, for emitters with a per-particle size, rotation, texture angle and
// alpha and one colour and texture scale. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/math/CVectorOps.h"
#include "game/UnkTU54.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// A per-particle value, or one value for every particle when there is no array.
template <class T>
struct UnkPerParticle_l186830 {
    T value;
    T* values;
    const T& Get(u32 i) const { return values ? values[i] : value; }
};

struct UnkColor_l186830 {
    u32 r, g, b;
};

struct UnkEmitter_l186830 {
    u8 pad_0x0[0x88];
    CVector3D* positions;  // 0x88
    u8 pad_0x90[0x10];
    UnkPerParticle_l186830<CVector3D> rotation;  // 0xa0
    u8 pad_0xb8[0x10];
    UnkPerParticle_l186830<CVector3D> sizeA;  // 0xc8
    u8 pad_0xe0[0x10];
    UnkPerParticle_l186830<CVector3D> sizeB;  // 0xf0
    u8 pad_0x108[0x38];
    UnkPerParticle_l186830<UnkColor_l186830> color;  // 0x140
    u8 pad_0x158[0x10];
    UnkPerParticle_l186830<u32> alpha;  // 0x168
    u8 pad_0x178[0x10];
    CVector2D uvPivot;  // 0x188
    u8 pad_0x190[0x18];
    UnkPerParticle_l186830<f32> angle;  // 0x1a8
    u8 pad_0x1b8[0x10];
    UnkPerParticle_l186830<CVector2D> uvScale;  // 0x1c8
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
static_assert(__builtin_offsetof(UnkEmitter_l186830, sizeA) == 0xc8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l186830, color) == 0x140, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l186830, alpha) == 0x168, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l186830, angle) == 0x1a8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l186830, uvScale) == 0x1c8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l186830, sizeFlags) == 0x348, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l186830, count) == 0x390, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l186830, field_0x3f0) == 0x3f0, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l186830, field_0x43a) == 0x43a, "layout");

// One vertex of the frame's vertex buffer. The buffer is raw memory the renderer reads back, so its
// fields may alias anything.
struct __attribute__((may_alias)) UnkVertex_l186830 {
    CVector3D pos;
    f32 u;
    f32 v;
    u32 color[4];
};

struct UnkOwner_710016fa84;
void fn_710016fa84(UnkOwner_710016fa84* self, const CVector3D* up, const CVector3D* rot,
                   CVector3D* side, CVector3D* fwd);

extern void* g_unk_716e34c3d0;

// One corner's position: the particle's position moved along the two quad axes.
inline void SetPos_l186830(UnkVertex_l186830* vertex, const CVector3D* pos, const CVector3D& right,
                           const CVector3D& down, const CVector2D& corner) {
    vertex->pos.x = pos->x + right.x * corner.x + down.x * corner.y;
    vertex->pos.y = pos->y + right.y * corner.x + down.y * corner.y;
    vertex->pos.z = pos->z + right.z * corner.x + down.z * corner.y;
}

// The texture coordinates of one corner: its point on the unit square turned by the particle's angle,
// moved by the pivot and scaled about the centre of the texture.
inline void SetUV_l186830(UnkVertex_l186830* vertex, const UnkEmitter_l186830* self, f32 u, f32 v) {
    vertex->u = (u - self->uvPivot.x) * self->uvScale.value.x + 0.5f;
    vertex->v = (v - self->uvPivot.y) * self->uvScale.value.y + 0.5f;
}

void fn_7100186830(UnkEmitter_l186830* self, const UnkVec3f& dir) {
    UnkVertex_l186830* vertices = (UnkVertex_l186830*)g_unk_716e34c3d0;
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
        up = TransformDirectionByMatrix3x3(self->field_0x3f0, up);
        f32 inv = 1.0f / (sqrtf(up.x * up.x + up.y * up.y + up.z * up.z) + 0.0001f);
        up.x *= inv;
        up.y *= inv;
        up.z *= inv;
    }

    u32 count = self->count;
    f32 add = (f32)(flags & 2);
    f32 mul = (f32)(flags & 1);
    f32 addX = scaleX * add;
    f32 addY = scaleY * add;
    f32 mulX = scaleX * mul;
    f32 mulY = scaleY * mul;

    for (u32 i = 0; i < count; i++) {
        UnkVertex_l186830* v = &vertices[i * 4];
        const u32& index = self->indices[i];
        const CVector3D* a = &self->sizeA.Get(index);
        const CVector3D* b = &self->sizeB.Get(index);
        f32 width = addX * (a->x + b->x) + mulX * (a->x * b->x);
        f32 height = addY * (a->y + b->y) + mulY * (a->y * b->y);
        const CVector3D* pos = &self->positions[index];
        fn_710016fa84((UnkOwner_710016fa84*)self, (const CVector3D*)&up, &self->rotation.Get(index),
                      &side, &fwd);
        CVector3D right = fwd * width;
        CVector3D down = side * height;
        SetPos_l186830(&v[0], pos, right, down, self->corners[0]);
        SetPos_l186830(&v[1], pos, right, down, self->corners[1]);
        SetPos_l186830(&v[2], pos, right, down, self->corners[2]);
        SetPos_l186830(&v[3], pos, right, down, self->corners[3]);

        const UnkColor_l186830& color = self->color.value;
        const u32* alpha = &self->alpha.Get(index);
        v[0].color[0] = v[1].color[0] = v[2].color[0] = v[3].color[0] = color.r;
        v[0].color[1] = v[1].color[1] = v[2].color[1] = v[3].color[1] = color.g;
        v[0].color[2] = v[1].color[2] = v[2].color[2] = v[3].color[2] = color.b;
        v[0].color[3] = v[1].color[3] = v[2].color[3] = v[3].color[3] = *alpha;

        f32 angle = self->angle.Get(index);
        f32 s = sinf(angle);
        f32 c = cosf(angle);
        f32 u0 = c * 0.5f - s * 0.5f;
        f32 v0 = s * -0.5f + c * -0.5f;
        f32 u1 = c * -0.5f - s * 0.5f;
        f32 v1 = s * 0.5f + c * -0.5f;
        f32 u2 = c * 0.5f - s * -0.5f;
        f32 v2 = s * -0.5f + c * 0.5f;
        f32 u3 = c * -0.5f - s * -0.5f;
        f32 v3 = s * 0.5f + c * 0.5f;
        SetUV_l186830(&v[0], self, u0, v0);
        SetUV_l186830(&v[1], self, u1, v1);
        SetUV_l186830(&v[2], self, u2, v2);
        SetUV_l186830(&v[3], self, u3, v3);
    }
}
