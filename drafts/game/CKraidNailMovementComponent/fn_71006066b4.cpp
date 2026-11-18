// CKraidNailMovementComponent's unit (tools/tu_map.py unit 929): after the base update (fn_7100ac3a78)
// the nail spins about z by dt * 2pi * its speed's share of 1500 (capped at 1) times the class's rate
// (+0x134 of the class at +0x5d8), the way its velocity's x points. Names and layouts are placeholders.
#include "game/CProjectileMovement.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkNailongThornMovement;

struct UnkNailOwner_l6066b4 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x3c0 - 0x94];
    bool dirty;  // 0x3c0
};

struct UnkNailClass_l6066b4 {
    u8 pad_0x0[0x134];
    f32 spin_rate;  // 0x134
};

struct UnkNailMovement_l6066b4 {
    u8 pad_0x0[0x30];
    UnkNailOwner_l6066b4* owner;  // 0x30
    u8 pad_0x38[0x4b8 - 0x38];
    f32 vel_x;  // 0x4b8
    f32 vel_y;  // 0x4bc
    f32 vel_z;  // 0x4c0
    u8 pad_0x4c4[0x5d8 - 0x4c4];
    UnkNailClass_l6066b4* char_class;  // 0x5d8
};

// The speed's share of the top speed, 0 to 1.
inline f32 SpeedShare(f32 speed) {
    const f32 kTopSpeed = 1500.0f;
    if (speed <= 0.0f)
        return 0.0f;
    if (speed > kTopSpeed)
        speed = kTopSpeed;
    return speed / kTopSpeed;
}

void fn_71006066b4(UnkNailMovement_l6066b4* self, f32 dt) {
    fn_7100ac3a78(reinterpret_cast<UnkNailongThornMovement*>(self), dt);
    UnkVec3f angle = self->owner->xform.GetAngle();
    f32 speed = __builtin_sqrtf(self->vel_x * self->vel_x + self->vel_y * self->vel_y + self->vel_z * self->vel_z);
    f32 share = SpeedShare(speed);
    f32 turn = dt * 6.2831855f * share;
    f32 rate = self->char_class->spin_rate;
    f32 delta = self->vel_x < 0.0f ? rate * turn : -(rate * turn);
    angle.z = delta + angle.z;
    UnkNailOwner_l6066b4* owner = self->owner;
    owner->xform.SetAngle(angle);
    owner->dirty = true;
}
