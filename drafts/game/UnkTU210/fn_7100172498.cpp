// Unit 210 (a GPU particle system's emitters): writes four camera-facing vertices per live particle
// into the shared vertex buffer; this variant has one size and alpha for all, and a per-particle
// colour, UV offset, UV scale and UV angle. Names and layouts are placeholders.
#include <math.h>

#include <nn/util/util_MathTypes.h>

#include "base/math/CVector.h"
#include "game/UnkTU54.h"
#include "types.h"

struct UnkOwner_710016fa84;
void fn_710016fa84(UnkOwner_710016fa84* self, const base::math::CVector3D* up,
                   const base::math::CVector3D* rot, base::math::CVector3D* side,
                   base::math::CVector3D* fwd);

extern void* g_unk_716e34c3d0;

struct UnkVec2f_l172498 {
    f32 x, y;
};

// One per-particle attribute: its default value and the per-particle values (null when unused).
template <class T>
struct UnkParticleAttr_l172498 {
    u8 pad_0x0[0x10];
    T value;
    T* data;

    const T& Get(u32 particle) const { return data ? data[particle] : value; }
};

struct UnkEmitter_l172498 {
    u8 pad_0x0[0x68];
    UnkParticleAttr_l172498<UnkVec3f> position;          // 0x68
    UnkParticleAttr_l172498<UnkVec3f> rotation;          // 0x90
    UnkParticleAttr_l172498<UnkVec3f> size;              // 0xb8
    UnkParticleAttr_l172498<UnkVec3f> size_scale;        // 0xe0
    UnkParticleAttr_l172498<UnkVec3f> field_0x108;
    UnkParticleAttr_l172498<UnkVec3f> color;             // 0x130
    UnkParticleAttr_l172498<f32> alpha;                  // 0x158
    UnkParticleAttr_l172498<UnkVec2f_l172498> uv_offset; // 0x178
    UnkParticleAttr_l172498<f32> uv_angle;               // 0x198
    UnkParticleAttr_l172498<UnkVec2f_l172498> uv_scale;  // 0x1b8
    u8 pad_0x1d8[0x348 - 0x1d8];
    s32 size_mode;  // 0x348: bit 0 multiplies the two sizes, bit 1 adds them
    UnkVec2f_l172498 corners[4];  // 0x34c
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
static_assert(__builtin_offsetof(UnkEmitter_l172498, uv_scale) == 0x1b8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l172498, indices) == 0x398, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l172498, field_0x43a) == 0x43a, "layout");

struct UnkParticleVertex_l172498 {
    nn::util::Float3 pos;
    nn::util::Float2 uv;
    nn::util::Float4 color;
};

struct UnkParticleQuad_l172498 {
    UnkParticleVertex_l172498 v[4];
};

// Where one corner of the quad sits relative to the particle.
inline UnkVec3f CornerOffset(const UnkVec3f& across, const UnkVec3f& up, const UnkVec2f_l172498& corner) {
    return UnkVec3f(across.x * corner.x + up.x * corner.y, across.y * corner.x + up.y * corner.y,
                    across.z * corner.x + up.z * corner.y);
}

inline void SetPosition(UnkParticleVertex_l172498& v, const UnkVec3f& offset, const UnkVec3f& p) {
    v.pos.x = offset.x + p.x;
    v.pos.y = offset.y + p.y;
    v.pos.z = offset.z + p.z;
}

void fn_7100172498(UnkEmitter_l172498* self, const UnkVec3f& view) {
    UnkParticleQuad_l172498* out = static_cast<UnkParticleQuad_l172498*>(g_unk_716e34c3d0);
    UnkVec3f side(1.0f, 0.0f, 0.0f);
    UnkVec3f fwd(0.0f, 1.0f, 0.0f);
    UnkVec3f dir = view;
    if (!self->field_0x43a) {
        dir = TransformDirectionByMatrix3x3(self->matrix, dir);
        f32 inv = 1.0f / (sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z) + 0.0001f);
        dir.x *= inv;
        dir.y *= inv;
        dir.z *= inv;
    }
    f32 size_x = self->size_x;
    f32 size_y = self->size_y;
    s32 mode = self->size_mode;
    fn_710016fa84(reinterpret_cast<UnkOwner_710016fa84*>(self),
                  reinterpret_cast<const base::math::CVector3D*>(&dir),
                  reinterpret_cast<const base::math::CVector3D*>(&self->rotation.value),
                  reinterpret_cast<base::math::CVector3D*>(&side),
                  reinterpret_cast<base::math::CVector3D*>(&fwd));

    // Every particle has the same size, so the corner offsets are the same for all.
    f32 add = mode & 2;
    f32 mul = mode & 1;
    const UnkVec3f& a = self->size.value;
    const UnkVec3f& b = self->size_scale.value;
    f32 w = size_x * add * (a.x + b.x) + size_x * mul * (a.x * b.x);
    f32 h = size_y * add * (a.y + b.y) + size_y * mul * (a.y * b.y);
    UnkVec3f across(fwd.x * w, fwd.y * w, fwd.z * w);
    UnkVec3f up(side.x * h, side.y * h, side.z * h);
    UnkVec3f offset0 = CornerOffset(across, up, self->corners[0]);
    UnkVec3f offset1 = CornerOffset(across, up, self->corners[1]);
    UnkVec3f offset2 = CornerOffset(across, up, self->corners[2]);
    UnkVec3f offset3 = CornerOffset(across, up, self->corners[3]);

    s32 count = self->count;
    for (s32 i = 0; i < count; ++i) {
        UnkParticleVertex_l172498* v = out[i].v;
        const u32& index = self->indices[i];

        const UnkVec3f& p = self->position.data[index];
        SetPosition(v[0], offset0, p);
        SetPosition(v[1], offset1, p);
        SetPosition(v[2], offset2, p);
        SetPosition(v[3], offset3, p);

        const UnkVec3f& color = self->color.Get(index);
        const f32& alpha = self->alpha.value;
        v[0].color.x = v[1].color.x = v[2].color.x = v[3].color.x = color.x;
        v[0].color.y = v[1].color.y = v[2].color.y = v[3].color.y = color.y;
        v[0].color.z = v[1].color.z = v[2].color.z = v[3].color.z = color.z;
        v[0].color.w = v[1].color.w = v[2].color.w = v[3].color.w = alpha;

        // The UV of each corner, rotated by the particle's angle about the texture's centre.
        const UnkVec2f_l172498& offset = self->uv_offset.Get(index);
        const UnkVec2f_l172498& scale = self->uv_scale.Get(index);
        f32 angle = self->uv_angle.Get(index);
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

        v[0].uv.x = (u0 - offset.x) * scale.x + 0.5f;
        v[0].uv.y = (v0 - offset.y) * scale.y + 0.5f;
        v[1].uv.x = (u1 - offset.x) * scale.x + 0.5f;
        v[1].uv.y = (v1 - offset.y) * scale.y + 0.5f;
        v[2].uv.x = (u2 - offset.x) * scale.x + 0.5f;
        v[2].uv.y = (v2 - offset.y) * scale.y + 0.5f;
        v[3].uv.x = (u3 - offset.x) * scale.x + 0.5f;
        v[3].uv.y = (v3 - offset.y) * scale.y + 0.5f;
    }
}
