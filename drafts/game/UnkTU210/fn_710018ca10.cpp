// A unit with no known name (unit 210): writes one camera-facing quad per live particle into the frame's
// vertex buffer, each particle with its own size, scale, colour and alpha when the emitter keeps them.
// Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector3D;

struct UnkOwner_710016fa84;

struct UnkQuadVertex_18ca10 {
    UnkVec3f pos;
    f32 uv[2];
    UnkVec3f color;
    u32 alpha;
};
static_assert(sizeof(UnkQuadVertex_18ca10) == 0x24, "");

struct UnkQuadCorner_18ca10 {
    f32 up;
    f32 side;
};

struct UnkEmitter_18ca10 {
    u8 pad_0x0[0x88];
    CVector3D* positions;
    u8 pad_0x90[0x10];
    CVector3D rotation;
    u8 pad_0xac[0x1c];
    CVector3D defaultSize;
    u8 pad_0xd4[4];
    CVector3D* sizes;
    u8 pad_0xe0[0x10];
    CVector3D defaultScale;
    u8 pad_0xfc[4];
    CVector3D* scales;
    u8 pad_0x108[0x38];
    UnkVec3f defaultColor;
    u8 pad_0x14c[4];
    UnkVec3f* colors;
    u8 pad_0x158[0x10];
    u32 defaultAlpha;
    u8 pad_0x16c[4];
    u32* alphas;
    u8 pad_0x178[0x10];
    f32 uvOffset[2];
    u8 pad_0x190[0x18];
    f32 uvAngle;
    u8 pad_0x1ac[0x1c];
    f32 uvScale[2];
    u8 pad_0x1d0[0x178];
    s32 sizeFlags;
    UnkQuadCorner_18ca10 corners[4];
    u8 pad_0x36c[8];
    f32 sizeX;
    f32 sizeY;
    u8 pad_0x37c[0x14];
    u32 count;
    u32* indices;
    u8 pad_0x3a0[0x50];
    UnkMatrix44 matrix;
    u8 pad_0x430[0xa];
    bool field_0x43a;
};
static_assert(__builtin_offsetof(UnkEmitter_18ca10, alphas) == 0x170, "");
static_assert(__builtin_offsetof(UnkEmitter_18ca10, sizeFlags) == 0x348, "");
static_assert(__builtin_offsetof(UnkEmitter_18ca10, count) == 0x390, "");
static_assert(__builtin_offsetof(UnkEmitter_18ca10, field_0x43a) == 0x43a, "");

extern void* g_unk_716e34c3d0;

void fn_710016fa84(UnkOwner_710016fa84* self, const CVector3D* up, const CVector3D* rot,
                   CVector3D* side, CVector3D* fwd);

// The particle's own value when the emitter keeps one per particle, else the emitter's.
template <class T>
inline const T& PerParticle(const T* values, const T& fallback, u32 index) {
    return values != nullptr ? values[index] : fallback;
}

void fn_710018ca10(UnkEmitter_18ca10* self, const UnkVec3f* dir) {
    UnkQuadVertex_18ca10* out = (UnkQuadVertex_18ca10*)g_unk_716e34c3d0;
    CVector3D side;
    side.x = 0.0f;
    side.y = 0.0f;
    side.z = 0.0f;
    CVector3D fwd;
    fwd.x = 0.0f;
    fwd.y = 0.0f;
    fwd.z = 0.0f;
    UnkVec3f up = *dir;
    if (!self->field_0x43a) {
        up = TransformDirectionByMatrix3x3(self->matrix, up);
        f32 inv = 1.0f / (sqrtf(up.x * up.x + up.y * up.y + up.z * up.z) + 0.0001f);
        up.x = up.x * inv;
        up.y = up.y * inv;
        up.z = up.z * inv;
    }
    fn_710016fa84((UnkOwner_710016fa84*)self, (const CVector3D*)&up, &self->rotation, &side, &fwd);

    f32 angle = self->uvAngle;
    f32 sizeX = self->sizeX;
    f32 sizeY = self->sizeY;
    s32 flags = self->sizeFlags;
    f32 s = sinf(angle);
    f32 c = cosf(angle);

    u32 count = self->count;
    if (count == 0)
        return;

    // The UV square's corners (+-0.5), rotated by the UV angle, then offset and scaled about its centre.
    f32 u0 = self->uvScale[0] * (0.5f * c - 0.5f * s - self->uvOffset[0]) + 0.5f;
    f32 v0 = (-0.5f * s + -0.5f * c - self->uvOffset[1]) * self->uvScale[1] + 0.5f;
    f32 u1 = self->uvScale[0] * (-0.5f * c - 0.5f * s - self->uvOffset[0]) + 0.5f;
    f32 v1 = (0.5f * s + -0.5f * c - self->uvOffset[1]) * self->uvScale[1] + 0.5f;
    f32 u2 = self->uvScale[0] * (0.5f * c - -0.5f * s - self->uvOffset[0]) + 0.5f;
    f32 v2 = (-0.5f * s + 0.5f * c - self->uvOffset[1]) * self->uvScale[1] + 0.5f;
    f32 u3 = self->uvScale[0] * (-0.5f * c - -0.5f * s - self->uvOffset[0]) + 0.5f;
    f32 v3 = (0.5f * s + 0.5f * c - self->uvOffset[1]) * self->uvScale[1] + 0.5f;

    f32 additive = flags & 2;
    f32 multiplied = flags & 1;
    for (u32 i = 0; i < count; i++) {
        u32 index = self->indices[i];
        const CVector3D& base = PerParticle(self->sizes, self->defaultSize, index);
        const CVector3D& scale = PerParticle(self->scales, self->defaultScale, index);
        f32 width = sizeX * additive * (base.x + scale.x) + sizeX * multiplied * (base.x * scale.x);
        f32 height = sizeY * additive * (base.y + scale.y) + sizeY * multiplied * (base.y * scale.y);
        CVector3D u = {fwd.x * width, width * fwd.y, width * fwd.z};
        CVector3D r = {height * side.x, height * side.y, height * side.z};
        const CVector3D& p = self->positions[index];
        for (s32 k = 0; k < 4; k++) {
            const UnkQuadCorner_18ca10& corner = self->corners[k];
            out[k].pos.x = p.x + u.x * corner.up + r.x * corner.side;
            out[k].pos.y = p.y + u.y * corner.up + r.y * corner.side;
            out[k].pos.z = p.z + u.z * corner.up + r.z * corner.side;
        }
        UnkVec3f color = PerParticle(self->colors, self->defaultColor, self->indices[i]);
        out[0].color = color;
        out[1].color = color;
        out[2].color = color;
        out[3].color = color;
        u32 alpha = PerParticle(self->alphas, self->defaultAlpha, self->indices[i]);
        out[0].uv[0] = u0;
        out[0].uv[1] = v0;
        out[0].alpha = alpha;
        out[1].uv[0] = u1;
        out[1].uv[1] = v1;
        out[1].alpha = alpha;
        out[2].uv[0] = u2;
        out[2].uv[1] = v2;
        out[2].alpha = alpha;
        out[3].uv[0] = u3;
        out[3].uv[1] = v3;
        out[3].alpha = alpha;
        out += 4;
    }
}
