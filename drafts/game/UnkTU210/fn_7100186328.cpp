// Unit 210 (a GPU particle system's emitters): writes one camera-facing quad of four vertices per live
// particle (position, rotated texture coordinates, colour) into the frame's vertex buffer.
// Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "game/UnkTU54.h"
#include "types.h"

using base::math::CVector3D;

struct UnkOwner_710016fa84;
void fn_710016fa84(UnkOwner_710016fa84* self, const CVector3D* up, const CVector3D* rot,
                   CVector3D* side, CVector3D* fwd);

extern void* g_unk_716e34c3d0;

struct UnkColor_l186328 {
    f32 r, g, b, a;
};

struct UnkVertex_l186328 {
    UnkVec3f pos;
    f32 u, v;
    UnkColor_l186328 color;
};
static_assert(sizeof(UnkVertex_l186328) == 0x24, "UnkVertex_l186328 layout");

// One per-particle attribute: its default value and the per-particle values (null when unused).
template <class T>
struct UnkParticleAttr_l186328 {
    u8 pad_0x0[0x10];
    T value;
    T* data;

    const T& Get(u32 particle) const { return data ? data[particle] : value; }
};

struct UnkCorner_l186328 {
    f32 x, y;
};

struct UnkEmitter_l186328 {
    u8 pad_0x0[0x68];
    UnkParticleAttr_l186328<UnkVec3f> position;  // 0x68
    u8 pad_0x90[0x10];
    CVector3D rotation;                         // 0xa0
    u8 pad_0xac[0x1c];
    f32 size_a[2];  // 0xc8
    u8 pad_0xd0[0x20];
    f32 size_b[2];  // 0xf0
    u8 pad_0xf8[0x38];
    UnkParticleAttr_l186328<UnkVec3f> color;  // 0x130
    u8 pad_0x158[0x10];
    f32 alpha;  // 0x168
    u8 pad_0x16c[0x1c];
    f32 uv_offset[2];  // 0x188
    u8 pad_0x190[0x8];
    UnkParticleAttr_l186328<f32> angle;  // 0x198
    u8 pad_0x1b8[0x10];
    f32 uv_scale[2];  // 0x1c8
    u8 pad_0x1d0[0x178];
    s32 flags;                      // 0x348
    UnkCorner_l186328 corners[4];   // 0x34c
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
static_assert(__builtin_offsetof(UnkEmitter_l186328, rotation) == 0xa0, "UnkEmitter_l186328 layout");
static_assert(__builtin_offsetof(UnkEmitter_l186328, color) == 0x130, "UnkEmitter_l186328 layout");
static_assert(__builtin_offsetof(UnkEmitter_l186328, angle) == 0x198, "UnkEmitter_l186328 layout");
static_assert(__builtin_offsetof(UnkEmitter_l186328, flags) == 0x348, "UnkEmitter_l186328 layout");
static_assert(__builtin_offsetof(UnkEmitter_l186328, count) == 0x390, "UnkEmitter_l186328 layout");
static_assert(__builtin_offsetof(UnkEmitter_l186328, matrix) == 0x3f0, "UnkEmitter_l186328 layout");
static_assert(__builtin_offsetof(UnkEmitter_l186328, world_space) == 0x43a, "UnkEmitter_l186328 layout");

// A corner of the texture, rotated about its centre (v points the other way).
static inline UnkCorner_l186328 Rotate(f32 s, f32 c, f32 x, f32 y) {
    UnkCorner_l186328 r;
    r.x = c * x - s * y;
    r.y = s * -x + c * -y;
    return r;
}

void fn_7100186328(UnkEmitter_l186328* self, const UnkVec3f& view) {
    CVector3D side;
    side.x = 1.0f;
    side.y = 0.0f;
    side.z = 0.0f;
    CVector3D fwd;
    fwd.x = 0.0f;
    fwd.y = 1.0f;
    fwd.z = 0.0f;
    UnkVertex_l186328* vtx = (UnkVertex_l186328*)g_unk_716e34c3d0;
    UnkVec3f dir = view;
    if (!self->world_space) {
        dir = fn_7100077c04(self->matrix, dir);
        f32 len = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
        f32 inv = 1.0f / (len + 0.0001f);
        dir.x = dir.x * inv;
        dir.y = inv * dir.y;
        dir.z = inv * dir.z;
    }
    f32 scale_x = self->scale_x;
    f32 scale_y = self->scale_y;
    s32 flags = self->flags;
    fn_710016fa84((UnkOwner_710016fa84*)self, (const CVector3D*)&dir, &self->rotation, &side, &fwd);

    u32 count = self->count;
    if (count == 0)
        return;

    f32 fa = flags & 2;
    f32 fb = flags & 1;
    f32 w = scale_x * fa * (self->size_a[0] + self->size_b[0]) +
            scale_x * fb * (self->size_a[0] * self->size_b[0]);
    f32 h = scale_y * fa * (self->size_a[1] + self->size_b[1]) +
            scale_y * fb * (self->size_a[1] * self->size_b[1]);
    CVector3D right = {fwd.x * w, w * fwd.y, w * fwd.z};
    CVector3D up = {h * side.x, h * side.y, h * side.z};

    CVector3D corner[4];
    for (s32 k = 0; k < 4; k++) {
        corner[k].x = right.x * self->corners[k].x + up.x * self->corners[k].y;
        corner[k].y = right.y * self->corners[k].x + up.y * self->corners[k].y;
        corner[k].z = right.z * self->corners[k].x + up.z * self->corners[k].y;
    }

    for (u32 i = 0; i < count; i++, vtx += 4) {
        u32 index = self->indices[i];
        const UnkVec3f& pos = self->position.data[index];
        for (s32 k = 0; k < 4; k++) {
            vtx[k].pos.x = corner[k].x + pos.x;
            vtx[k].pos.y = corner[k].y + pos.y;
            vtx[k].pos.z = corner[k].z + pos.z;
        }

        const UnkVec3f& rgb = self->color.Get(self->indices[i]);
        UnkColor_l186328 color = {rgb.x, rgb.y, rgb.z, self->alpha};
        vtx[3].color = color;
        vtx[2].color = color;
        vtx[1].color = color;
        vtx[0].color = color;

        f32 angle = self->angle.Get(self->indices[i]);
        f32 s = sinf(angle);
        f32 c = cosf(angle);
        UnkCorner_l186328 uv[4] = {
            Rotate(s, c, 0.5f, 0.5f),
            Rotate(s, c, -0.5f, 0.5f),
            Rotate(s, c, 0.5f, -0.5f),
            Rotate(s, c, -0.5f, -0.5f),
        };
        for (s32 k = 0; k < 4; k++) {
            vtx[k].u = (uv[k].x - self->uv_offset[0]) * self->uv_scale[0] + 0.5f;
            vtx[k].v = (uv[k].y - self->uv_offset[1]) * self->uv_scale[1] + 0.5f;
        }
    }
}
