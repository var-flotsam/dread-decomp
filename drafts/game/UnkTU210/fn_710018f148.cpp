// Unit 210 (a GPU particle system's emitters): writes one quad of four vertices per live particle,
// oriented by each particle's rotation and sized by its two size attributes, with one texture angle and
// one colour for the whole emitter. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "game/UnkTU54.h"
#include "types.h"

using base::math::CVector3D;

struct UnkOwner_710016fa84;
void fn_710016fa84(UnkOwner_710016fa84* self, const CVector3D* up, const CVector3D* rot,
                   CVector3D* side, CVector3D* fwd);

extern void* g_unk_716e34c3d0;

struct UnkColor_l18f148 {
    f32 r, g, b, a;
};

struct UnkVertex_l18f148 {
    UnkVec3f pos;
    f32 u, v;
    UnkColor_l18f148 color;
};
static_assert(sizeof(UnkVertex_l18f148) == 0x24, "UnkVertex_l18f148 layout");

// One per-particle attribute: its default value and the per-particle values (null when unused).
template <class T>
struct UnkParticleAttr_l18f148 {
    u8 pad_0x0[0x10];
    T value;
    T* data;

    const T* Get(u32 particle) const { return data ? &data[particle] : &value; }
};

struct UnkCorner_l18f148 {
    f32 x, y;
};

struct UnkEmitter_l18f148 {
    u8 pad_0x0[0x68];
    UnkParticleAttr_l18f148<UnkVec3f> position;  // 0x68
    UnkParticleAttr_l18f148<CVector3D> rotation;  // 0x90
    UnkParticleAttr_l18f148<UnkVec3f> size_a;     // 0xb8
    UnkParticleAttr_l18f148<UnkVec3f> size_b;     // 0xe0
    u8 pad_0x108[0x28];
    UnkParticleAttr_l18f148<UnkVec3f> color;  // 0x130
    u8 pad_0x158[0x10];
    f32 alpha;  // 0x168
    u8 pad_0x16c[0x1c];
    f32 uv_offset[2];  // 0x188
    u8 pad_0x190[0x8];
    UnkParticleAttr_l18f148<f32> angle;  // 0x198
    u8 pad_0x1b8[0x10];
    f32 uv_scale[2];  // 0x1c8
    u8 pad_0x1d0[0x178];
    s32 flags;                     // 0x348
    f32 corners[4][2];  // 0x34c: each corner's position along the two axes
    u8 pad_0x36c[0x8];
    f32 scale_x;  // 0x374
    f32 scale_y;  // 0x378
    u8 pad_0x37c[0x14];
    u32 count;     // 0x390
    u32* indices;  // 0x398
    u8 pad_0x3a0[0x50];
    UnkMatrix44 matrix;  // 0x3f0
    u8 pad_0x430[0xa];
    bool world_space;  // 0x43a
};
static_assert(__builtin_offsetof(UnkEmitter_l18f148, size_b) == 0xe0, "UnkEmitter_l18f148 layout");
static_assert(__builtin_offsetof(UnkEmitter_l18f148, color) == 0x130, "UnkEmitter_l18f148 layout");
static_assert(__builtin_offsetof(UnkEmitter_l18f148, angle) == 0x198, "UnkEmitter_l18f148 layout");
static_assert(__builtin_offsetof(UnkEmitter_l18f148, flags) == 0x348, "UnkEmitter_l18f148 layout");
static_assert(__builtin_offsetof(UnkEmitter_l18f148, count) == 0x390, "UnkEmitter_l18f148 layout");
static_assert(__builtin_offsetof(UnkEmitter_l18f148, matrix) == 0x3f0, "UnkEmitter_l18f148 layout");
static_assert(__builtin_offsetof(UnkEmitter_l18f148, world_space) == 0x43a, "UnkEmitter_l18f148 layout");

// A corner of the texture, rotated about its centre and mapped into the emitter's texture rectangle:
// u comes from the corner (ux, uy), v from the one opposite it (vx, vy), since v points the other way.
static inline UnkCorner_l18f148 TexCorner(const UnkEmitter_l18f148* self, f32 s, f32 c, f32 ux, f32 uy,
                                          f32 vx, f32 vy) {
    UnkCorner_l18f148 uv;
    uv.x = (c * ux - s * uy - self->uv_offset[0]) * self->uv_scale[0] + 0.5f;
    uv.y = (s * vx + c * vy - self->uv_offset[1]) * self->uv_scale[1] + 0.5f;
    return uv;
}

void fn_710018f148(UnkEmitter_l18f148* self, const UnkVec3f& view) {
    UnkVertex_l18f148* vtx = (UnkVertex_l18f148*)g_unk_716e34c3d0;
    CVector3D side;
    side.x = 0.0f;
    side.y = 0.0f;
    side.z = 0.0f;
    CVector3D fwd;
    fwd.x = 0.0f;
    fwd.y = 0.0f;
    fwd.z = 0.0f;
    s32 flags = self->flags;
    f32 scale_x = self->scale_x;
    f32 scale_y = self->scale_y;
    UnkVec3f dir = view;
    if (!self->world_space) {
        dir = TransformDirectionByMatrix3x3(self->matrix, dir);
        f32 len = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
        f32 inv = 1.0f / (len + 0.0001f);
        dir.x = dir.x * inv;
        dir.y = inv * dir.y;
        dir.z = inv * dir.z;
    }

    f32 angle = self->angle.value;
    f32 s = sinf(angle);
    f32 c = cosf(angle);

    u32 count = self->count;
    if (count == 0)
        return;

    UnkCorner_l18f148 uv0 = TexCorner(self, s, c, 0.5f, 0.5f, -0.5f, -0.5f);
    UnkCorner_l18f148 uv1 = TexCorner(self, s, c, -0.5f, 0.5f, 0.5f, -0.5f);
    UnkCorner_l18f148 uv2 = TexCorner(self, s, c, 0.5f, -0.5f, -0.5f, 0.5f);
    UnkCorner_l18f148 uv3 = TexCorner(self, s, c, -0.5f, -0.5f, 0.5f, 0.5f);
    f32 fa = flags & 2;
    f32 fb = flags & 1;

    for (s32 i = 0; i < count; i++, vtx += 4) {
        u32 index = self->indices[i];
        const UnkVec3f* a = self->size_a.Get(index);
        const UnkVec3f* b = self->size_b.Get(index);
        const UnkVec3f& pos = self->position.data[index];
        const CVector3D* rot = self->rotation.Get(index);
        f32 w = scale_x * fa * (a->x + b->x) + scale_x * fb * (a->x * b->x);
        f32 h = scale_y * fa * (a->y + b->y) + scale_y * fb * (a->y * b->y);
        fn_710016fa84((UnkOwner_710016fa84*)self, (const CVector3D*)&dir, rot, &side, &fwd);

        CVector3D right = {w * fwd.x, w * fwd.y, w * fwd.z};
        CVector3D up = {h * side.x, h * side.y, h * side.z};
        for (s32 k = 0; k < 4; k++) {
            vtx[k].pos.x = pos.x + right.x * self->corners[k][0] + up.x * self->corners[k][1];
            vtx[k].pos.y = pos.y + right.y * self->corners[k][0] + up.y * self->corners[k][1];
            vtx[k].pos.z = pos.z + right.z * self->corners[k][0] + up.z * self->corners[k][1];
        }

        const UnkVec3f& rgb = self->color.value;
        vtx[0].color.r = vtx[1].color.r = vtx[2].color.r = vtx[3].color.r = rgb.x;
        vtx[0].color.g = vtx[1].color.g = vtx[2].color.g = vtx[3].color.g = rgb.y;
        vtx[0].color.b = vtx[1].color.b = vtx[2].color.b = vtx[3].color.b = rgb.z;
        vtx[0].color.a = vtx[1].color.a = vtx[2].color.a = vtx[3].color.a = self->alpha;

        vtx[0].u = uv0.x;
        vtx[0].v = uv0.y;
        vtx[1].u = uv1.x;
        vtx[1].v = uv1.y;
        vtx[2].u = uv2.x;
        vtx[2].v = uv2.y;
        vtx[3].u = uv3.x;
        vtx[3].v = uv3.y;
    }
}
