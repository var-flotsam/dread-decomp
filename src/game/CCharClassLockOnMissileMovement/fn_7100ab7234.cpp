// CCharClassLockOnMissileMovement's unit (tools/tu_map.py, unit 1616): the movement's update. The step is
// clamped to [0, 0.016]; unless the missile is held (+0x608, while the owner's animation runs a fixed
// action), the time at +0x5d4 advances, the speed at +0x674 eases from +0x66c to +0x670 between the times
// at +0x664 and +0x668, and the velocity at +0x4b8 is rescaled to that speed. Then the base update
// fn_7100ac3a78 runs. Names and layouts are placeholders.
#include <math.h>

#include "game/CProjectileMovement.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkLockOnOwner_lab7234 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkLockOnMissileMovement_lab7234 {
    u8 pad_0x0[0x30];
    UnkLockOnOwner_lab7234* owner;  // 0x30
    u8 pad_0x38[0x4b8 - 0x38];
    f32 velocity_x;  // 0x4b8
    f32 velocity_y;
    f32 velocity_z;
    u8 pad_0x4c4[0x5d0 - 0x4c4];
    bool accelerated;  // 0x5d0
    u8 pad_0x5d1[3];
    f32 time;  // 0x5d4
    u8 pad_0x5d8[0x608 - 0x5d8];
    bool held;  // 0x608
    u8 pad_0x609[0x664 - 0x609];
    f32 ease_start;  // 0x664
    f32 ease_end;  // 0x668
    f32 speed_from;  // 0x66c
    f32 speed_to;  // 0x670
    f32 speed;  // 0x674
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_7100ab7234(UnkLockOnMissileMovement_lab7234* self, f32 dt) {
    f32 step = !(dt <= 0.0f) ? (!(dt >= 0.016f) ? dt : 0.016f) : 0.0f;
    if (self->held) {
        if (UnkAnimationComponent* animation = self->owner->animation)
            if (fn_710117ae38(animation, 0xcc49865919da8c93, false))
                return;
    } else {
        f32 t = step + self->time;
        f32 start = self->ease_start;
        f32 end = self->ease_end;
        f32 from = self->speed_from;
        f32 to = self->speed_to;
        self->time = t;
        if (t < start) {
            self->speed = from;
        } else if (!self->accelerated && t >= end) {
            if (t > end) {
                self->accelerated = true;
                self->speed = to;
            } else {
                f32 elapsed = t - start;
                f32 ease = sinf((1.0f / (end - start)) * elapsed);
                self->speed = from + (to - from) * ease;
            }
        }
        f32 length = sqrtf(self->velocity_x * self->velocity_x + self->velocity_y * self->velocity_y +
                           self->velocity_z * self->velocity_z);
        f32 inv = 1.0f / (length + 0.0001f);
        f32 x = self->velocity_x * inv;
        f32 y = self->velocity_y * inv;
        f32 z = inv * self->velocity_z;
        f32 speed = self->speed;
        self->velocity_x = x * speed;
        self->velocity_y = y * speed;
        self->velocity_z = z * speed;
    }
    fn_7100ac3a78(reinterpret_cast<UnkNailongThornMovement*>(self), step);
}
