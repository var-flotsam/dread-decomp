// CCameraCtrl unit (tools/tu_map.py unit 2693): moves the camera's current view towards its target
// view by `t` (snapping to it for a negative `t`), keeping the two directions unit length.
// Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector3D;

struct UnkCameraView_l0ee5d8 {
    CVector3D pos;
    CVector3D dir;
    CVector3D up;
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
};

struct UnkCameraBlend_l0ee5d8 {
    u8 pad_0x0[0x38];
    UnkCameraView_l0ee5d8 current;  // 0x38
    UnkCameraView_l0ee5d8 target;   // 0x68
};

static inline f32 Mix(f32 a, f32 b, f32 t) {
    return (1.0f - t) * a + b * t;
}

// `a` moved towards `b` by `t`, or `b` itself once `t` reaches 1.
static inline f32 Lerp(f32 a, f32 b, f32 t) {
    if (t >= 1.0f)
        return b;
    return Mix(a, b, t);
}

static inline CVector3D Lerp(const CVector3D& a, const CVector3D& b, f32 t) {
    if (t >= 1.0f)
        return b;
    return CVector3D{Mix(a.x, b.x, t), Mix(a.y, b.y, t), Mix(a.z, b.z, t)};
}

// `v` scaled to unit length (a zero vector stays zero).
static inline CVector3D Normalized(const CVector3D& v) {
    f32 inv = 1.0f / (sqrtf(v.z * v.z + (v.y * v.y + v.x * v.x)) + 0.0001f);
    return CVector3D{v.x * inv, v.y * inv, v.z * inv};
}

static inline bool Near(const CVector3D& a, const CVector3D& b) {
    return fabsf(a.x - b.x) < 0.0001f && fabsf(a.y - b.y) < 0.0001f && fabsf(a.z - b.z) < 0.0001f;
}

void fn_71010ee5d8(UnkCameraBlend_l0ee5d8* self, f32 t) {
    if (t < 0.0f) {
        self->current.pos = self->target.pos;
        self->current.dir = self->target.dir;
        self->current.up = self->target.up;
        self->current.field_0x24 = self->target.field_0x24;
        self->current.field_0x28 = self->target.field_0x28;
        self->current.field_0x2c = self->target.field_0x2c;
        return;
    }
    // The position snaps to the target once it is close enough.
    self->current.pos = Near(self->current.pos, self->target.pos)
                            ? self->target.pos
                            : Lerp(self->current.pos, self->target.pos, t);
    self->current.dir = Normalized(Lerp(self->current.dir, self->target.dir, t));
    self->current.up = Normalized(Lerp(self->current.up, self->target.up, t));
    self->current.field_0x24 = Lerp(self->current.field_0x24, self->target.field_0x24, t);
    self->current.field_0x28 = Lerp(self->current.field_0x28, self->target.field_0x28, t);
    self->current.field_0x2c = Lerp(self->current.field_0x2c, self->target.field_0x2c, t);
}
