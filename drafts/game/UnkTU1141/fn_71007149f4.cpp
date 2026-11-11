// Unit 1141 (animation channels): fn_71007149f4 blends another weighted transform channel into this
// one by `t`, summing the weights (flag set) or keeping the larger one; an empty channel takes the
// other's transform scaled by `t`. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "base/math/UnkZeroVector3.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector3D;
using base::math::CVector4D;
using base::math::UnkVector4D;

// A transform with a channel mask, a translation and a weight.
struct UnkWeighted_7100714808 {
    UnkXform xform;     // 0x0
    u32 mask;           // 0x5c
    f32 tx, ty, tz;     // 0x60
    f32 weight;         // 0x6c
};
static_assert(sizeof(UnkWeighted_7100714808) == 0x70, "UnkWeighted_7100714808 layout");

__attribute__((visibility("hidden"))) extern u32 g_unk_717270ec80;
extern const CVector3D g_unk_71016477a8;

void fn_7100714604(UnkWeighted_7100714808* self, const UnkWeighted_7100714808* other);
UnkMatrix44 fn_7100078834(const UnkMatrix44* a, const UnkMatrix44* b, f32 t);
UnkVector4D fn_71000772fc(const UnkMatrix44* m);
UnkVector4D fn_710007b5c0(const CVector4D* a, f32 s);
CVector4D fn_7100079fb0(const CVector4D* q);
UnkMatrix44 fn_7100078b24(const CVector3D* translation, const CVector4D* rotation, const CVector3D* scale);

struct UnkVec3 {
    f32 x, y, z;
};
static inline UnkVec3 Translation(const UnkWeighted_7100714808* w) { return UnkVec3{w->tx, w->ty, w->tz}; }
static inline UnkVec3 Scale(f32 s, const UnkVec3& v) { return UnkVec3{s * v.x, s * v.y, s * v.z}; }
static inline UnkVec3 Scale(const UnkVec3& v, f32 s) { return UnkVec3{v.x * s, v.y * s, v.z * s}; }
static inline UnkVec3 operator+(const UnkVec3& a, const UnkVec3& b) { return UnkVec3{a.x + b.x, a.y + b.y, a.z + b.z}; }

// The rotation of `m` scaled by `t`.
static inline UnkVector4D PartialRotation(const UnkMatrix44& m, f32 t) {
    UnkVector4D q = fn_71000772fc(&m);
    return fn_710007b5c0(&q, t);
}

static inline bool IsActive(const UnkWeighted_7100714808* w) { return w->weight > 0.0f && w->mask != 0; }

static inline const UnkMatrix44& MatrixOf(const UnkWeighted_7100714808* w) {
    return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(w));
}

static inline void BlendIn(UnkWeighted_7100714808* self, const UnkWeighted_7100714808* other, f32 t, u32 mode) {
    if (t == 1.0f) {
        fn_7100714604(self, other);
        return;
    }
    if (!IsActive(self)) {
        CVector3D translation;
        translation.x = other->xform.m[0][3] * t;
        translation.y = other->xform.m[1][3] * t;
        translation.z = other->xform.m[2][3] * t;
        CVector4D rotation;
        {
            UnkVector4D scaled = PartialRotation(MatrixOf(other), t);
            UnkVector4D arg(scaled);
            rotation = fn_7100079fb0(&arg);
        }
        UnkMatrix44 m = fn_7100078b24(&translation, &rotation, &g_unk_71016477a8);
        if (!(other->weight > 0.0f) || other->mask == 0)
            return;
        f32 weight = other->weight;
        self->weight = weight;
        u32 mask = other->mask;
        if (mask & 2) {
            f32 inv = 1.0f / weight;
            self->tx = inv * m.m[3];
            self->ty = inv * m.m[7];
            self->tz = inv * m.m[11];
        } else {
            self->tx = g_unk_710164779c.x;
            self->ty = g_unk_710164779c.y;
            self->tz = g_unk_710164779c.z;
        }
        UnkCopyMatrix(reinterpret_cast<UnkMatrix44*>(self->xform.m), m);
        self->xform.flags = (self->xform.flags | 8) & ~g_unk_717270ec80;
        self->mask = mask;
        return;
    }
    const UnkMatrix44& a = MatrixOf(self);
    const UnkMatrix44& b = MatrixOf(other);
    UnkMatrix44 m = fn_7100078834(&a, &b, t);
    UnkCopyMatrix(reinterpret_cast<UnkMatrix44*>(self->xform.m), m);
    self->xform.flags = (self->xform.flags | 8) & ~g_unk_717270ec80;
    self->mask = other->mask | self->mask;
    f32 wa = self->weight;
    f32 wb = other->weight;
    if (mode == 1) {
        f32 total = wa + wb;
        f32 s = 1.0f - t;
        f32 inv = 1.0f / total;
        self->tx = inv * (wa * (s * self->tx)) + inv * (wb * (other->tx * t));
        self->ty = inv * (wa * (s * self->ty)) + inv * (wb * (other->ty * t));
        self->tz = inv * (wa * (s * self->tz)) + inv * (wb * (other->tz * t));
        self->weight = total;
    } else {
        f32 top = wa > wb ? wa : wb;
        f32 inv = 1.0f / top;
        self->tx = inv * m.m[3];
        self->ty = inv * m.m[7];
        self->tz = inv * m.m[11];
        self->weight = top;
    }
}

void fn_71007149f4(UnkWeighted_7100714808* self, const UnkWeighted_7100714808* other, f32 t, u32 mode) {
    if (IsActive(other) && t > 0.0f)
        BlendIn(self, other, t, mode);
}
