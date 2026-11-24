// Unit 210 (GPU particle system): writes one camera-facing textured quad per live particle into the
// frame's vertex buffer, every quad the same size, with a per-particle alpha stream.
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
template <typename T>
struct UnkParticleStream_18f654 {
    T value;
    u32 pad;
    T* data;

    const T* Get(u32 index) const { return data ? &data[index] : &value; }
};

struct UnkParticleVertex_18f654 {
    f32 pos[3];
    f32 uv[2];       // 0xc
    UnkVec3f color;  // 0x14
    f32 alpha;       // 0x20
};
static_assert(sizeof(UnkParticleVertex_18f654) == 0x24, "vertex size");

struct UnkQuadEmitter_18f654 {
    u8 pad_0x0[0x78];
    UnkParticleStream_18f654<CVector3D> positions;  // 0x78
    u8 pad_0x90[0x10];
    CVector3D rotation;                  // 0xa0
    u8 pad_0xac[0x1c];
    UnkParticleStream_18f654<CVector3D> sizeA;  // 0xc8
    u8 pad_0xe0[0x10];
    UnkParticleStream_18f654<CVector3D> sizeB;  // 0xf0
    u8 pad_0x108[0x38];
    UnkVec3f color;  // 0x140
    u8 pad_0x14c[0x1c];
    UnkParticleStream_18f654<f32> alpha;  // 0x168
    u8 pad_0x178[0x10];
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
static_assert(__builtin_offsetof(UnkQuadEmitter_18f654, color) == 0x140, "");
static_assert(__builtin_offsetof(UnkQuadEmitter_18f654, alpha) == 0x168, "");
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

static inline void SetUv(UnkParticleVertex_18f654* vertex, const CVector2D& uv) {
    vertex->uv[0] = uv.x;
    vertex->uv[1] = uv.y;
}

static inline CVector3D MakeVector(f32 x, f32 y, f32 z) {
    CVector3D v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

// The offset of quad corner `corner` from the particle's position.
static inline CVector3D CornerOffset(const CVector2D& corner, const CVector3D& right, const CVector3D& up) {
    CVector3D offset;
    offset.x = right.x * corner.x + up.x * corner.y;
    offset.y = right.y * corner.x + up.y * corner.y;
    offset.z = right.z * corner.x + up.z * corner.y;
    return offset;
}

static inline void SetPosition(UnkParticleVertex_18f654* vertex, const CVector3D& offset, const CVector3D* p) {
    vertex->pos[0] = offset.x + p->x;
    vertex->pos[1] = offset.y + p->y;
    vertex->pos[2] = offset.z + p->z;
}

void fn_710018ec78(UnkQuadEmitter_18f654* self, const UnkVec3f* direction) {
    UnkParticleVertex_18f654* out = (UnkParticleVertex_18f654*)g_unk_716e34c3d0;
    CVector3D side = MakeVector(1.0f, 0.0f, 0.0f);
    CVector3D fwd = MakeVector(0.0f, 1.0f, 0.0f);
    UnkVec3f dir = *direction;
    if (!self->worldSpace) {
        dir = TransformDirectionByMatrix3x3(self->transform, dir);
        f32 inv = InvLengthEps(dir);
        dir.x = dir.x * inv;
        dir.y = inv * dir.y;
        dir.z = inv * dir.z;
    }
    f32 scaleX = self->sizeScale.x;
    f32 scaleY = self->sizeScale.y;
    s32 mode = self->sizeMode;
    fn_710016fa84((UnkOwner_710016fa84*)self, (const CVector3D*)&dir, &self->rotation, &side, &fwd);

    f32 ax = self->sizeA.value.x;
    f32 ay = self->sizeA.value.y;
    f32 angle = self->uvAngle;
    f32 bx = self->sizeB.value.x;
    f32 by = self->sizeB.value.y;
    f32 sum = mode & 2;
    f32 product = mode & 1;
    f32 width = scaleX * sum * (ax + bx) + scaleX * product * (ax * bx);
    f32 height = scaleY * sum * (ay + by) + scaleY * product * (ay * by);
    CVector3D right = {fwd.x * width, fwd.y * width, fwd.z * width};
    CVector3D up = {side.x * height, side.y * height, side.z * height};
    CVector3D offset0 = CornerOffset(self->corners[0], right, up);
    CVector3D offset1 = CornerOffset(self->corners[1], right, up);
    CVector3D offset2 = CornerOffset(self->corners[2], right, up);
    CVector3D offset3 = CornerOffset(self->corners[3], right, up);

    f32 s = sinf(angle);
    f32 c = cosf(angle);
    CVector2D uv0 = CornerUv(self, s, c, 0.5f, 0.5f);
    CVector2D uv1 = CornerUv(self, s, c, -0.5f, 0.5f);
    CVector2D uv2 = CornerUv(self, s, c, 0.5f, -0.5f);
    CVector2D uv3 = CornerUv(self, s, c, -0.5f, -0.5f);

    u32 count = self->count;
    for (u32 i = 0; i < count; i++) {
        const CVector3D* p = &self->positions.data[self->indices[i]];
        SetPosition(&out[0], offset0, p);
        SetPosition(&out[1], offset1, p);
        SetPosition(&out[2], offset2, p);
        SetPosition(&out[3], offset3, p);
        const f32* alpha = self->alpha.Get(self->indices[i]);
        out[0].color = out[1].color = out[2].color = out[3].color = self->color;
        out[0].alpha = out[1].alpha = out[2].alpha = out[3].alpha = *alpha;
        SetUv(&out[0], uv0);
        SetUv(&out[1], uv1);
        SetUv(&out[2], uv2);
        SetUv(&out[3], uv3);
        out += 4;
    }
}
