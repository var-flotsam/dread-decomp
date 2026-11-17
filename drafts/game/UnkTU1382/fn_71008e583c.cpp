// Unit 1382: steps an animated set of values by `dt`. Each value moves from its start toward its target
// over its duration; the two vectors tell the object after every step (virtual slots 0x78 and 0x80), the
// colours (fn_71008c6b0c) and the numbers only while they still differ from their targets (slots 0x88
// to 0xa8). Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "types.h"

using base::math::CVector3D;
using base::math::CVector4D;
using base::math::UnkVector4D;

struct UnkColorLerp_A4_b75;
UnkVector4D fn_71008c6b0c(UnkColorLerp_A4_b75* self, f32 dt);

static inline bool Near(f32 a, f32 b) {
    return a - 1e-5f <= b && a + 1e-5f >= b;
}

// The same test with the two bounds compared in the other order (as the original does for the last
// component).
static inline bool NearW(f32 a, f32 b) {
    return a + 1e-5f >= b && a - 1e-5f <= b;
}

// The same test with both bounds taken first (as the original does for a vector's z).
static inline bool Within(f32 a, f32 b) {
    f32 lo = a - 1e-5f;
    f32 hi = a + 1e-5f;
    return hi >= b && lo <= b;
}

// How far along the duration the elapsed time is, at most all the way.
static inline f32 Progress(f32 elapsed, f32 duration) {
    f32 t = elapsed / duration;
    return t > 1.0f ? 1.0f : t;
}

struct UnkTween3_l8e583c {
    CVector3D current;
    CVector3D start;
    CVector3D target;
    f32 duration;
    f32 elapsed;

    bool Done() const {
        return Near(current.x, target.x) && Near(current.y, target.y) && Within(current.z, target.z);
    }
    void Step(f32 dt) {
        f32 t = elapsed + dt;
        elapsed = t;
        if (t < duration) {
            f32 k = Progress(t, duration);
            f32 dx = target.x - start.x;
            f32 dy = target.y - start.y;
            f32 dz = target.z - start.z;
            dx = k * dx;
            dy = k * dy;
            dz = k * dz;
            f32 x = start.x + dx;
            f32 y = start.y + dy;
            f32 z = start.z + dz;
            current.x = x;
            current.y = y;
            current.z = z;
        } else {
            current.x = target.x;
            current.y = target.y;
            current.z = target.z;
        }
    }
};

struct UnkTween4_l8e583c {
    CVector4D current;
    CVector4D start;
    CVector4D target;
    f32 duration;
    f32 elapsed;

    bool Done() const {
        return Near(current.x, target.x) && Near(current.y, target.y) && Near(current.z, target.z) &&
               NearW(current.w, target.w);
    }
};

struct UnkTween1_l8e583c {
    f32 current;
    f32 start;
    f32 target;
    f32 duration;
    f32 elapsed;

    bool Done() const { return NearW(current, target); }
    f32 Step(f32 dt) {
        f32 t = elapsed + dt;
        elapsed = t;
        if (t < duration) {
            f32 k = Progress(t, duration);
            return start + k * (target - start);
        }
        return target;
    }
};

class UnkTweenSet_l8e583c {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();

    u8 pad_0x8[0x14];
    UnkTween3_l8e583c field_0x1c;
    UnkTween3_l8e583c field_0x48;
    UnkTween4_l8e583c field_0x74;
    UnkTween4_l8e583c field_0xac;
    UnkTween1_l8e583c field_0xe4;
    UnkTween1_l8e583c field_0xf8;
    UnkTween1_l8e583c field_0x10c;
};
static_assert(__builtin_offsetof(UnkTweenSet_l8e583c, field_0x74) == 0x74, "layout");
static_assert(__builtin_offsetof(UnkTweenSet_l8e583c, field_0x10c) == 0x10c, "layout");

void fn_71008e583c(UnkTweenSet_l8e583c* self, f32 dt) {
    if (!self->field_0x1c.Done())
        self->field_0x1c.Step(dt);
    self->vf_0x78();
    if (!self->field_0x48.Done())
        self->field_0x48.Step(dt);
    self->vf_0x80();
    if (!self->field_0x74.Done()) {
        fn_71008c6b0c(reinterpret_cast<UnkColorLerp_A4_b75*>(&self->field_0x74), dt);
        self->vf_0x88();
    }
    if (!self->field_0xac.Done()) {
        fn_71008c6b0c(reinterpret_cast<UnkColorLerp_A4_b75*>(&self->field_0xac), dt);
        self->vf_0x90();
    }
    if (!self->field_0xe4.Done()) {
        self->field_0xe4.current = self->field_0xe4.Step(dt);
        self->vf_0x98();
    }
    if (!self->field_0xf8.Done()) {
        self->field_0xf8.current = self->field_0xf8.Step(dt);
        self->vf_0xa0();
    }
    if (!self->field_0x10c.Done()) {
        self->field_0x10c.current = self->field_0x10c.Step(dt);
        self->vf_0xa8();
    }
}
