// CFakePhysicsMovement unit (unit 2687 of tools/tu_map.py): resets the movement's state to its defaults
// (gravity 1500 down, a drag of 50, the velocity, direction and angles zero), with a random spin
// between 0 and 3.14. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector3D;

extern "C" int rand();

struct UnkFakePhysicsMovement_l0cf2e4 {
    u8 pad_0x0[0x231];
    bool field_0x231;
    u8 pad_0x232[2];
    u32 field_0x234;
    u8 pad_0x238[0x30];
    f32 field_0x268;
    CVector3D field_0x26c;
    bool field_0x278;
    u8 pad_0x279[3];
    f32 field_0x27c;
    CVector3D gravity;  // 0x280
    CVector3D velocity;  // 0x28c
    f32 field_0x298;
    f32 field_0x29c;
    u32 field_0x2a0;
    u32 field_0x2a4;
    bool field_0x2a8;
    bool field_0x2a9;
    bool field_0x2aa;
    bool field_0x2ab;
    f32 spin;  // 0x2ac
    f32 field_0x2b0;
    f32 field_0x2b4;
    bool field_0x2b8;
    u8 pad_0x2b9[3];
    CVector3D field_0x2bc;
    CVector3D direction;  // 0x2c8
    f32 angles[3];  // 0x2d4
    u32 field_0x2e0;
    u32 field_0x2e4;
    u32 field_0x2e8;
    bool field_0x2ec;
    bool field_0x2ed;
    u8 pad_0x2ee[2];
    u32 field_0x2f0;
    u32 field_0x2f4;
    bool field_0x2f8;
    u8 pad_0x2f9[3];
    u32 field_0x2fc;
};
static_assert(__builtin_offsetof(UnkFakePhysicsMovement_l0cf2e4, field_0x2fc) == 0x2fc, "layout");

// A random number in [lo, hi].
static inline f32 RandRange(f32 lo, f32 hi) {
    f32 t = (f32)((f64)(rand() & 0x7fff) * (1.0 / 32767.0));
    return lo + (hi - lo) * t;
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71010cf2e4(UnkFakePhysicsMovement_l0cf2e4* self) {
    self->field_0x278 = true;
    self->field_0x231 = true;
    self->field_0x234 = 0;
    self->gravity.x = 0.0f;
    self->gravity.y = -1500.0f;
    self->gravity.z = 0.0f;
    self->velocity.x = 0.0f;
    self->velocity.y = 0.0f;
    self->velocity.z = 0.0f;
    self->field_0x298 = -1.0f;
    self->field_0x2a4 = 0;
    self->field_0x2f4 = 0;
    self->field_0x2f8 = false;
    self->direction.x = 0.0f;
    self->direction.y = 0.0f;
    self->direction.z = 0.0f;
    self->angles[0] = 0.0f;
    self->angles[1] = 0.0f;
    self->angles[2] = 0.0f;
    self->field_0x2e0 = 0;
    self->field_0x2e4 = 0;
    self->field_0x2e8 = 0;
    self->field_0x2fc = 7;
    self->field_0x26c.x = 0.0f;
    self->field_0x26c.y = -1500.0f;
    self->field_0x26c.z = 0.0f;
    self->field_0x268 = 1.0f;
    self->field_0x27c = 50.0f;
    self->field_0x2a8 = true;
    self->field_0x2a9 = true;
    self->field_0x2aa = true;
    self->field_0x2ab = true;
    self->spin = RandRange(0.0f, 3.14f);
    self->field_0x2b0 = 0.0f;
    self->field_0x2b4 = -1.57f;
    self->field_0x2ec = false;
    self->field_0x2ed = true;
    self->field_0x2b8 = false;
    self->field_0x2bc.x = 0.0f;
    self->field_0x2bc.y = 0.0f;
    self->field_0x2bc.z = 0.0f;
    self->field_0x29c = 0.5f;
    self->field_0x2a0 = 5;
    self->field_0x2f0 = 0;
}
