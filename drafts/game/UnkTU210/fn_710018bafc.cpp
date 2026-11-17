// A unit with no known name (unit 210): writes one camera-facing quad per live particle into the frame's
// vertex buffer, each particle with its own size, scale and UV scale when the emitter keeps them.
// Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkOwner_710016fa84;

struct UnkQuadVertex_18bafc {
    f32 pos[3];
    f32 uv[2];
    UnkVec3f color;
    u32 alpha;
};
static_assert(sizeof(UnkQuadVertex_18bafc) == 0x24, "");

struct UnkQuadCorner_18bafc {
    f32 up;
    f32 side;
};

struct UnkEmitter_18bafc {
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
    CVector2D uvScale;
    CVector2D* uvScales;
    u8 pad_0x1d8[0x170];
    s32 sizeFlags;
    UnkQuadCorner_18bafc corners[4];
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
static_assert(__builtin_offsetof(UnkEmitter_18bafc, uvScales) == 0x1d0, "");
static_assert(__builtin_offsetof(UnkEmitter_18bafc, sizeFlags) == 0x348, "");
static_assert(__builtin_offsetof(UnkEmitter_18bafc, count) == 0x390, "");
static_assert(__builtin_offsetof(UnkEmitter_18bafc, field_0x43a) == 0x43a, "");

extern void* g_unk_716e34c3d0;

void fn_710016fa84(UnkOwner_710016fa84* self, const CVector3D* up, const CVector3D* rot,
                   CVector3D* side, CVector3D* fwd);

// The particle's own value when the emitter keeps one per particle, else the emitter's.
template <class T>
inline const T& PerParticle(const T* values, const T& fallback, u32 index) {
    return values != nullptr ? values[index] : fallback;
}

void fn_710018bafc(UnkEmitter_18bafc* self, const UnkVec3f* dir) {
    UnkQuadVertex_18bafc* out = (UnkQuadVertex_18bafc*)g_unk_716e34c3d0;
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
        up = fn_7100077c04(self->matrix, up);
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

    s32 count = self->count;
    if (count == 0)
        return;

    f32 additive = flags & 2;
    f32 multiplied = flags & 1;
    // The UV square's corners (+-0.5), rotated by the UV angle and offset; each particle scales them.
    f32 u0 = 0.5f * c - 0.5f * s - self->uvOffset[0];
    f32 v0 = -0.5f * s + -0.5f * c - self->uvOffset[1];
    f32 u1 = -0.5f * c - 0.5f * s - self->uvOffset[0];
    f32 v1 = 0.5f * s + -0.5f * c - self->uvOffset[1];
    f32 u2 = 0.5f * c - -0.5f * s - self->uvOffset[0];
    f32 v2 = -0.5f * s + 0.5f * c - self->uvOffset[1];
    f32 u3 = -0.5f * c - -0.5f * s - self->uvOffset[0];
    f32 v3 = 0.5f * s + 0.5f * c - self->uvOffset[1];

    for (s32 i = 0; i < count; i++) {
        u32 index = self->indices[i];
        const CVector3D& base = PerParticle(self->sizes, self->defaultSize, index);
        const CVector3D& scale = PerParticle(self->scales, self->defaultScale, index);
        f32 width = sizeX * additive * (base.x + scale.x) + sizeX * multiplied * (base.x * scale.x);
        f32 height = sizeY * additive * (base.y + scale.y) + sizeY * multiplied * (base.y * scale.y);
        CVector3D u = {fwd.x * width, width * fwd.y, width * fwd.z};
        CVector3D r = {height * side.x, height * side.y, height * side.z};
        const CVector3D& p = self->positions[index];
        for (s32 k = 0; k < 4; k++) {
            const UnkQuadCorner_18bafc& corner = self->corners[k];
            out[k].pos[0] = p.x + u.x * corner.up + r.x * corner.side;
            out[k].pos[1] = p.y + u.y * corner.up + r.y * corner.side;
            out[k].pos[2] = p.z + u.z * corner.up + r.z * corner.side;
        }
        UnkVec3f color = self->defaultColor;
        out[0].color = color;
        out[1].color = color;
        out[2].color = color;
        out[3].color = color;
        u32 alpha = self->defaultAlpha;
        out[0].alpha = alpha;
        out[1].alpha = alpha;
        out[2].alpha = alpha;
        out[3].alpha = alpha;
        const CVector2D& uvScale = PerParticle(self->uvScales, self->uvScale, self->indices[i]);
        out[0].uv[0] = u0 * uvScale.x + 0.5f;
        out[0].uv[1] = v0 * uvScale.y + 0.5f;
        out[1].uv[0] = u1 * uvScale.x + 0.5f;
        out[1].uv[1] = v1 * uvScale.y + 0.5f;
        out[2].uv[0] = u2 * uvScale.x + 0.5f;
        out[2].uv[1] = v2 * uvScale.y + 0.5f;
        out[3].uv[0] = u3 * uvScale.x + 0.5f;
        out[3].uv[1] = v3 * uvScale.y + 0.5f;
        out += 4;
    }
}
