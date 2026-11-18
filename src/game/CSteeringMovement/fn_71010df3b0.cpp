// CSteeringMovement's unit (tools/tu_map.py unit 2690): moves the owner's transform along the steering
// direction (+0x40) at the speed (+0x7c) for the time step, and marks the transform changed (+0x3b0).
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "types.h"

struct UnkSteeringVec3_l0df3b0 {
    f32 x;
    f32 y;
    f32 z;
    UnkSteeringVec3_l0df3b0(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkSteeringVec3_l0df3b0(const UnkSteeringVec3_l0df3b0& o) : x(o.x), y(o.y), z(o.z) {}
    UnkSteeringVec3_l0df3b0 operator*(f32 s) const { return UnkSteeringVec3_l0df3b0(x * s, y * s, z * s); }
    UnkSteeringVec3_l0df3b0 operator+(const UnkSteeringVec3_l0df3b0& o) const {
        return UnkSteeringVec3_l0df3b0(x + o.x, y + o.y, z + o.z);
    }
};

struct UnkSteeringOwner_l0df3b0 {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;  // 0x38: the translation is m[3], m[7], m[11]
    u8 pad_0x78[0x3b0 - 0x78];
    bool xform_changed;  // 0x3b0

    UnkSteeringVec3_l0df3b0 Translation() const { return UnkSteeringVec3_l0df3b0(xform.m[3], xform.m[7], xform.m[11]); }
    void SetTranslation(const UnkSteeringVec3_l0df3b0& t) {
        xform_changed = true;
        xform.m[3] = t.x;
        xform.m[7] = t.y;
        xform.m[11] = t.z;
    }
};
static_assert(__builtin_offsetof(UnkSteeringOwner_l0df3b0, xform_changed) == 0x3b0, "layout");

struct UnkSteeringMovement_l0df3b0 {
    u8 pad_0x0[0x30];
    UnkSteeringOwner_l0df3b0* owner;  // 0x30
    u8 pad_0x38[0x8];
    f32 direction[3];  // 0x40
    u8 pad_0x4c[0x7c - 0x4c];
    f32 speed;  // 0x7c

    UnkSteeringVec3_l0df3b0 Velocity() const {
        return UnkSteeringVec3_l0df3b0(speed * direction[0], speed * direction[1], speed * direction[2]);
    }
};
static_assert(__builtin_offsetof(UnkSteeringMovement_l0df3b0, speed) == 0x7c, "layout");

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71010df3b0(UnkSteeringMovement_l0df3b0* self, f32 dt) {
    UnkSteeringVec3_l0df3b0 step = self->Velocity() * dt;
    UnkSteeringOwner_l0df3b0* owner = self->owner;
    owner->SetTranslation(owner->Translation() + step);
}
