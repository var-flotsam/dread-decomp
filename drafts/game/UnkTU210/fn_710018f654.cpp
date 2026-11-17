// Unit 210 (GPU particle system): writes one camera-facing textured quad per live particle into the
// frame's vertex buffer, sized from two per-particle size streams and turned by the emitter's angle.
// Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "game/UnkTU54.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkOwner_710016fa84;

extern void* g_unk_716e34c3d0;

void fn_710016fa84(UnkOwner_710016fa84* self, const CVector3D* up, const CVector3D* rot,
                   CVector3D* side, CVector3D* fwd);

// A per-particle attribute: one value for every particle, or an array indexed by particle.
struct UnkParticleStream_18f654 {
    CVector3D value;
    u32 pad_0xc;
    CVector3D* data;  // 0x10

    const CVector3D* Get(u32 index) const { return data ? &data[index] : &value; }
};

struct UnkParticleVertex_18f654 {
    f32 pos[3];
    f32 uv[2];     // 0xc
    f32 color[4];  // 0x14: red, green, blue, alpha
};
static_assert(sizeof(UnkParticleVertex_18f654) == 0x24, "vertex size");

struct UnkQuadEmitter_18f654 {
    u8 pad_0x0[0x78];
    UnkParticleStream_18f654 positions;  // 0x78
    u8 pad_0x90[0x10];
    CVector3D rotation;                  // 0xa0
    u8 pad_0xac[0x1c];
    UnkParticleStream_18f654 sizeA;  // 0xc8
    u8 pad_0xe0[0x10];
    UnkParticleStream_18f654 sizeB;  // 0xf0
    u8 pad_0x108[0x38];
    f32 r, g, b;  // 0x140
    u8 pad_0x14c[0x1c];
    f32 a;  // 0x168
    u8 pad_0x16c[0x1c];
    CVector2D uvPivot;  // 0x188
    u8 pad_0x190[0x18];
    f32 uvAngle;  // 0x1a8
    u8 pad_0x1ac[0x1c];
    CVector2D uvScale;  // 0x1c8
    u8 pad_0x1d0[0x178];
    s32 sizeMode;          // 0x348
    CVector2D corners[4];  // 0x34c
    u8 pad_0x36c[0x8];
    CVector2D sizeScale;  // 0x374
    u8 pad_0x37c[0x14];
    u32 count;     // 0x390
    u32* indices;  // 0x398
    u8 pad_0x3a0[0x50];
    UnkMatrix44 transform;  // 0x3f0
    u8 pad_0x430[0xa];
    bool worldSpace;  // 0x43a
};
static_assert(__builtin_offsetof(UnkQuadEmitter_18f654, rotation) == 0xa0, "");
static_assert(__builtin_offsetof(UnkQuadEmitter_18f654, sizeB) == 0xf0, "");
static_assert(__builtin_offsetof(UnkQuadEmitter_18f654, r) == 0x140, "");
static_assert(__builtin_offsetof(UnkQuadEmitter_18f654, a) == 0x168, "");
static_assert(__builtin_offsetof(UnkQuadEmitter_18f654, uvPivot) == 0x188, "");
static_assert(__builtin_offsetof(UnkQuadEmitter_18f654, uvAngle) == 0x1a8, "");
static_assert(__builtin_offsetof(UnkQuadEmitter_18f654, uvScale) == 0x1c8, "");
static_assert(__builtin_offsetof(UnkQuadEmitter_18f654, sizeMode) == 0x348, "");
static_assert(__builtin_offsetof(UnkQuadEmitter_18f654, sizeScale) == 0x374, "");
static_assert(__builtin_offsetof(UnkQuadEmitter_18f654, count) == 0x390, "");
static_assert(__builtin_offsetof(UnkQuadEmitter_18f654, transform) == 0x3f0, "");
static_assert(__builtin_offsetof(UnkQuadEmitter_18f654, worldSpace) == 0x43a, "");

// Texture coordinates of quad corner (u, v) turned by the angle (sine s, cosine c) about the pivot.
static inline CVector2D CornerUv(const UnkQuadEmitter_18f654* self, f32 s, f32 c, f32 u, f32 v) {
    CVector2D uv;
    uv.x = self->uvScale.x * (c * u - s * v - self->uvPivot.x) + 0.5f;
    uv.y = (s * -u + c * -v - self->uvPivot.y) * self->uvScale.y + 0.5f;
    return uv;
}

static inline void SetCorner(UnkParticleVertex_18f654* vertex, const CVector3D* p,
                             const CVector2D* corner, const CVector3D& right, const CVector3D& up) {
    vertex->pos[0] = p->x + right.x * corner->x + up.x * corner->y;
    vertex->pos[1] = p->y + right.y * corner->x + up.y * corner->y;
    vertex->pos[2] = p->z + right.z * corner->x + up.z * corner->y;
}

static inline void SetUv(UnkParticleVertex_18f654* vertex, const CVector2D& uv) {
    vertex->uv[0] = uv.x;
    vertex->uv[1] = uv.y;
}

void fn_710018f654(UnkQuadEmitter_18f654* self, const UnkVec3f* direction) {
    CVector3D side = {};
    UnkParticleVertex_18f654* out = (UnkParticleVertex_18f654*)g_unk_716e34c3d0;
    CVector3D fwd = {};
    UnkVec3f dir = *direction;
    if (!self->worldSpace) {
        dir = fn_7100077c04(self->transform, dir);
        dir = Normalized(dir);
    }
    fn_710016fa84((UnkOwner_710016fa84*)self, (const CVector3D*)&dir, &self->rotation, &side, &fwd);

    f32 angle = self->uvAngle;
    f32 scaleX = self->sizeScale.x;
    f32 scaleY = self->sizeScale.y;
    s32 mode = self->sizeMode;
    f32 s = sinf(angle);
    f32 c = cosf(angle);
    CVector2D uv0 = CornerUv(self, s, c, 0.5f, 0.5f);
    CVector2D uv1 = CornerUv(self, s, c, -0.5f, 0.5f);
    CVector2D uv2 = CornerUv(self, s, c, 0.5f, -0.5f);
    CVector2D uv3 = CornerUv(self, s, c, -0.5f, -0.5f);
    f32 sum = mode & 2;
    f32 product = mode & 1;

    u32 count = self->count;
    for (u32 i = 0; i < count; i++) {
        u32 index = self->indices[i];
        const CVector3D* a = self->sizeA.Get(index);
        const CVector3D* b = self->sizeB.Get(index);
        const CVector3D* p = &self->positions.data[index];
        f32 width = scaleX * sum * (a->x + b->x) + scaleX * product * (a->x * b->x);
        f32 height = scaleY * sum * (a->y + b->y) + scaleY * product * (a->y * b->y);
        CVector3D right = {fwd.x * width, fwd.y * width, fwd.z * width};
        CVector3D up = {side.x * height, side.y * height, side.z * height};

        SetCorner(&out[0], p, &self->corners[0], right, up);
        SetCorner(&out[1], p, &self->corners[1], right, up);
        SetCorner(&out[2], p, &self->corners[2], right, up);
        SetCorner(&out[3], p, &self->corners[3], right, up);
        out[0].color[0] = out[1].color[0] = out[2].color[0] = out[3].color[0] = self->r;
        out[0].color[1] = out[1].color[1] = out[2].color[1] = out[3].color[1] = self->g;
        out[0].color[2] = out[1].color[2] = out[2].color[2] = out[3].color[2] = self->b;
        out[0].color[3] = out[1].color[3] = out[2].color[3] = out[3].color[3] = self->a;
        SetUv(&out[0], uv0);
        SetUv(&out[1], uv1);
        SetUv(&out[2], uv2);
        SetUv(&out[3], uv3);
        out += 4;
    }
}
