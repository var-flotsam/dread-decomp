// Unit 210 (a GPU particle system's emitters): writes four camera-facing vertices per live particle
// into the shared vertex buffer, sized by two per-particle size attributes and with rotated UVs.
// Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkOwner_710016fa84;
void fn_710016fa84(UnkOwner_710016fa84* self, const base::math::CVector3D* up,
                   const base::math::CVector3D* rot, base::math::CVector3D* side,
                   base::math::CVector3D* fwd);

extern void* g_unk_716e34c3d0;

struct UnkVec2f_l17df98 {
    f32 x, y;
};

// One per-particle attribute: its default value and the per-particle values (null when unused).
template <class T>
struct UnkParticleAttr_l17df98 {
    u8 pad_0x0[0x10];
    T value;
    T* data;

    const T& Get(u32 particle) const { return data ? data[particle] : value; }
};

struct UnkEmitter_l17df98 {
    u8 pad_0x0[0x68];
    UnkParticleAttr_l17df98<UnkVec3f> position;          // 0x68
    UnkParticleAttr_l17df98<UnkVec3f> rotation;          // 0x90
    UnkParticleAttr_l17df98<UnkVec3f> size;              // 0xb8
    UnkParticleAttr_l17df98<UnkVec3f> size_scale;        // 0xe0
    UnkParticleAttr_l17df98<UnkVec3f> field_0x108;
    UnkParticleAttr_l17df98<UnkVec3f> color;             // 0x130
    UnkParticleAttr_l17df98<f32> alpha;                  // 0x158
    UnkParticleAttr_l17df98<UnkVec2f_l17df98> uv_offset; // 0x178
    UnkParticleAttr_l17df98<f32> uv_angle;               // 0x198
    UnkParticleAttr_l17df98<UnkVec2f_l17df98> uv_scale;  // 0x1b8
    u8 pad_0x1d8[0x348 - 0x1d8];
    s32 size_mode;  // 0x348: bit 0 multiplies the two sizes, bit 1 adds them
    UnkVec2f_l17df98 corners[4];  // 0x34c
    u8 pad_0x36c[0x374 - 0x36c];
    f32 size_x;  // 0x374
    f32 size_y;  // 0x378
    u8 pad_0x37c[0x390 - 0x37c];
    u32 count;  // 0x390
    u8 pad_0x394[4];
    u32* indices;  // 0x398
    u8 pad_0x3a0[0x3f0 - 0x3a0];
    UnkMatrix44 matrix;  // 0x3f0
    u8 pad_0x430[0xa];
    bool field_0x43a;
};
static_assert(__builtin_offsetof(UnkEmitter_l17df98, uv_scale) == 0x1b8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l17df98, indices) == 0x398, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l17df98, field_0x43a) == 0x43a, "layout");

struct UnkParticleVertex_l17df98 {
    f32 pos[3];
    f32 uv[2];
    f32 color[4];
};

struct UnkParticleQuad_l17df98 {
    UnkParticleVertex_l17df98 v[4];
};

void fn_710017df98(UnkEmitter_l17df98* self, const UnkVec3f& view) {
    UnkParticleQuad_l17df98* out = static_cast<UnkParticleQuad_l17df98*>(g_unk_716e34c3d0);
    UnkVec3f side(0.0f, 0.0f, 0.0f);
    UnkVec3f fwd(0.0f, 0.0f, 0.0f);
    UnkVec3f dir = view;
    if (!self->field_0x43a) {
        dir = TransformDirectionByMatrix3x3(self->matrix, dir);
        f32 inv = 1.0f / (sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z) + 0.0001f);
        dir.x *= inv;
        dir.y *= inv;
        dir.z *= inv;
    }
    fn_710016fa84(reinterpret_cast<UnkOwner_710016fa84*>(self),
                  reinterpret_cast<const base::math::CVector3D*>(&dir),
                  reinterpret_cast<const base::math::CVector3D*>(&self->rotation.value),
                  reinterpret_cast<base::math::CVector3D*>(&side),
                  reinterpret_cast<base::math::CVector3D*>(&fwd));

    f32 angle = self->uv_angle.value;
    f32 size_x = self->size_x;
    f32 size_y = self->size_y;
    s32 mode = self->size_mode;
    f32 s = sinf(angle);
    f32 c = cosf(angle);

    u32 count = self->count;
    f32 add = mode & 2;
    f32 mul = mode & 1;
    f32 add_x = size_x * add;
    f32 add_y = size_y * add;
    f32 mul_x = size_x * mul;
    f32 mul_y = size_y * mul;

    // The UV of each corner, rotated by the angle about the texture's centre.
    f32 u0 = c * 0.5f - s * 0.5f;
    f32 v0 = s * -0.5f + c * -0.5f;
    f32 u1 = c * -0.5f - s * 0.5f;
    f32 v1 = s * 0.5f + c * -0.5f;
    f32 u2 = c * 0.5f - s * -0.5f;
    f32 v2 = s * -0.5f + c * 0.5f;
    f32 u3 = c * -0.5f - s * -0.5f;
    f32 v3 = s * 0.5f + c * 0.5f;

    for (u32 i = 0; i < count; ++i) {
        const u32& index = self->indices[i];
        UnkParticleVertex_l17df98* v = out[i].v;

        const UnkVec3f& p = self->position.data[index];
        const UnkVec3f& a = self->size.Get(index);
        const UnkVec3f& b = self->size_scale.Get(index);
        f32 h = add_y * (a.y + b.y) + mul_y * (a.y * b.y);
        f32 w = add_x * (a.x + b.x) + mul_x * (a.x * b.x);
        UnkVec3f across(fwd.x * w, fwd.y * w, fwd.z * w);
        UnkVec3f up(side.x * h, side.y * h, side.z * h);
        for (int k = 0; k < 4; ++k) {
            const UnkVec2f_l17df98& corner = self->corners[k];
            v[k].pos[0] = p.x + across.x * corner.x + up.x * corner.y;
            v[k].pos[1] = p.y + across.y * corner.x + up.y * corner.y;
            v[k].pos[2] = p.z + across.z * corner.x + up.z * corner.y;
        }

        const UnkVec3f& color = self->color.Get(index);
        for (int k = 0; k < 4; ++k) {
            v[k].color[0] = color.x;
            v[k].color[1] = color.y;
            v[k].color[2] = color.z;
            v[k].color[3] = self->alpha.value;
        }

        const UnkVec2f_l17df98& offset = self->uv_offset.Get(index);
        v[0].uv[0] = (u0 - offset.x) * self->uv_scale.value.x + 0.5f;
        v[0].uv[1] = (v0 - offset.y) * self->uv_scale.value.y + 0.5f;
        v[1].uv[0] = (u1 - offset.x) * self->uv_scale.value.x + 0.5f;
        v[1].uv[1] = (v1 - offset.y) * self->uv_scale.value.y + 0.5f;
        v[2].uv[0] = (u2 - offset.x) * self->uv_scale.value.x + 0.5f;
        v[2].uv[1] = (v2 - offset.y) * self->uv_scale.value.y + 0.5f;
        v[3].uv[0] = (u3 - offset.x) * self->uv_scale.value.x + 0.5f;
        v[3].uv[1] = (v3 - offset.y) * self->uv_scale.value.y + 0.5f;
    }
}
