// Unit 210 (a GPU particle system's emitters): writes one quad of four vertices per live particle,
// facing one direction for the whole emitter, sized by each particle's two size attributes, with one
// texture angle and per-particle colour, alpha and texture scale. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "game/UnkTU54.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkOwner_710016fa84;
void fn_710016fa84(UnkOwner_710016fa84* self, const CVector3D* up, const CVector3D* rot,
                   CVector3D* side, CVector3D* fwd);

extern void* g_unk_716e34c3d0;

// A vertex in the frame's GPU vertex buffer, which is written as raw memory (it may alias anything).
struct __attribute__((may_alias)) UnkVertex_l188c98 {
    f32 pos[3];
    f32 uv[2];
    f32 color[4];
};
static_assert(sizeof(UnkVertex_l188c98) == 0x24, "UnkVertex_l188c98 layout");

// One per-particle attribute: its default value and the per-particle values (null when unused).
template <class T>
struct UnkParticleAttr_l188c98 {
    u8 pad_0x0[0x10];
    T value;
    T* data;

    const T* Get(u32 particle) const { return data ? &data[particle] : &value; }
};

struct UnkEmitter_l188c98 {
    u8 pad_0x0[0x68];
    UnkParticleAttr_l188c98<UnkVec3f> position;  // 0x68
    UnkParticleAttr_l188c98<CVector3D> rotation;  // 0x90
    UnkParticleAttr_l188c98<UnkVec3f> size_a;     // 0xb8
    UnkParticleAttr_l188c98<UnkVec3f> size_b;     // 0xe0
    u8 pad_0x108[0x28];
    UnkParticleAttr_l188c98<UnkVec3f> color;  // 0x130
    UnkParticleAttr_l188c98<f32> alpha;       // 0x158
    u8 pad_0x178[0x10];
    f32 uv_offset[2];  // 0x188
    u8 pad_0x190[0x8];
    UnkParticleAttr_l188c98<f32> angle;  // 0x198
    UnkParticleAttr_l188c98<CVector2D> uv_scale;  // 0x1b8
    u8 pad_0x1d8[0x170];
    s32 flags;          // 0x348
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
static_assert(__builtin_offsetof(UnkEmitter_l188c98, size_b) == 0xe0, "UnkEmitter_l188c98 layout");
static_assert(__builtin_offsetof(UnkEmitter_l188c98, alpha) == 0x158, "UnkEmitter_l188c98 layout");
static_assert(__builtin_offsetof(UnkEmitter_l188c98, angle) == 0x198, "UnkEmitter_l188c98 layout");
static_assert(__builtin_offsetof(UnkEmitter_l188c98, uv_scale) == 0x1b8, "UnkEmitter_l188c98 layout");
static_assert(__builtin_offsetof(UnkEmitter_l188c98, flags) == 0x348, "UnkEmitter_l188c98 layout");
static_assert(__builtin_offsetof(UnkEmitter_l188c98, count) == 0x390, "UnkEmitter_l188c98 layout");
static_assert(__builtin_offsetof(UnkEmitter_l188c98, matrix) == 0x3f0, "UnkEmitter_l188c98 layout");
static_assert(__builtin_offsetof(UnkEmitter_l188c98, world_space) == 0x43a, "UnkEmitter_l188c98 layout");

struct UnkTexCorner_l188c98 {
    f32 u, v;
};

// A corner of the texture, rotated about its centre and moved by the emitter's texture offset:
// u comes from the corner (ux, uy), v from the one opposite it (vx, vy), since v points the other way.
static inline UnkTexCorner_l188c98 TexCorner(const UnkEmitter_l188c98* self, f32 s, f32 c, f32 ux,
                                             f32 uy, f32 vx, f32 vy) {
    UnkTexCorner_l188c98 t;
    t.u = c * ux - s * uy - self->uv_offset[0];
    t.v = s * vx + c * vy - self->uv_offset[1];
    return t;
}

// Scales a texture corner by the particle's texture scale about the texture's centre.
static inline void SetUV(UnkVertex_l188c98& vtx, const UnkTexCorner_l188c98& tex, const CVector2D* scale) {
    vtx.uv[0] = tex.u * scale->x + 0.5f;
    vtx.uv[1] = tex.v * scale->y + 0.5f;
}

void fn_7100188c98(UnkEmitter_l188c98* self, const UnkVec3f& view) {
    UnkVertex_l188c98* vtx = (UnkVertex_l188c98*)g_unk_716e34c3d0;
    CVector3D side;
    side.x = 0.0f;
    side.y = 0.0f;
    side.z = 0.0f;
    CVector3D fwd;
    fwd.x = 0.0f;
    fwd.y = 0.0f;
    fwd.z = 0.0f;
    UnkVec3f dir = view;
    if (!self->world_space) {
        dir = TransformDirectionByMatrix3x3(self->matrix, dir);
        f32 len = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
        f32 inv = 1.0f / (len + 0.0001f);
        dir.x = dir.x * inv;
        dir.y = inv * dir.y;
        dir.z = inv * dir.z;
    }
    fn_710016fa84((UnkOwner_710016fa84*)self, (const CVector3D*)&dir, &self->rotation.value, &side, &fwd);

    f32 angle = self->angle.value;
    f32 scale_x = self->scale_x;
    f32 scale_y = self->scale_y;
    s32 flags = self->flags;
    f32 s = sinf(angle);
    f32 c = cosf(angle);

    u32 count = self->count;
    if (count == 0)
        return;

    UnkTexCorner_l188c98 tex0 = TexCorner(self, s, c, 0.5f, 0.5f, -0.5f, -0.5f);
    UnkTexCorner_l188c98 tex1 = TexCorner(self, s, c, -0.5f, 0.5f, 0.5f, -0.5f);
    UnkTexCorner_l188c98 tex2 = TexCorner(self, s, c, 0.5f, -0.5f, -0.5f, 0.5f);
    UnkTexCorner_l188c98 tex3 = TexCorner(self, s, c, -0.5f, -0.5f, 0.5f, 0.5f);
    f32 fa = flags & 2;
    f32 fb = flags & 1;

    for (u32 i = 0; i < count; i++, vtx += 4) {
        u32 index = self->indices[i];
        const UnkVec3f* a = self->size_a.Get(index);
        const UnkVec3f* b = self->size_b.Get(index);
        const UnkVec3f& pos = self->position.data[index];
        f32 w = scale_x * fa * (a->x + b->x) + scale_x * fb * (a->x * b->x);
        f32 h = scale_y * fa * (a->y + b->y) + scale_y * fb * (a->y * b->y);

        CVector3D right = {fwd.x * w, w * fwd.y, w * fwd.z};
        CVector3D up = {h * side.x, h * side.y, h * side.z};
        for (s32 k = 0; k < 4; k++) {
            vtx[k].pos[0] = pos.x + right.x * self->corners[k][0] + up.x * self->corners[k][1];
            vtx[k].pos[1] = pos.y + right.y * self->corners[k][0] + up.y * self->corners[k][1];
            vtx[k].pos[2] = pos.z + right.z * self->corners[k][0] + up.z * self->corners[k][1];
        }

        const UnkVec3f* rgb = self->color.Get(self->indices[i]);
        const f32* alpha = self->alpha.Get(self->indices[i]);
        vtx[0].color[0] = vtx[1].color[0] = vtx[2].color[0] = vtx[3].color[0] = rgb->x;
        vtx[0].color[1] = vtx[1].color[1] = vtx[2].color[1] = vtx[3].color[1] = rgb->y;
        vtx[0].color[2] = vtx[1].color[2] = vtx[2].color[2] = vtx[3].color[2] = rgb->z;
        vtx[0].color[3] = vtx[1].color[3] = vtx[2].color[3] = vtx[3].color[3] = *alpha;

        const CVector2D* scale = self->uv_scale.Get(self->indices[i]);
        SetUV(vtx[0], tex0, scale);
        SetUV(vtx[1], tex1, scale);
        SetUV(vtx[2], tex2, scale);
        SetUV(vtx[3], tex3, scale);
    }
}
