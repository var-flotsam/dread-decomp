// CSideEnemyMovement unit (tools/tu_map.py unit 1471): the rotation step. Unless the character class
// allows interpolated turns, it turns the angle y towards its limit by the class's speed
// (fn_7100964f8c) and the facing sign (+0x70), and eases the angle z towards its rest value, which
// is snapped when close to zero; without the turn flag (or with rotation ignored while moving) only
// angle y is set, to a quarter turn. Names and layouts are placeholders.
#include <math.h>

#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkEnemyMovement_b45a2;
f32 fn_7100964f8c(UnkEnemyMovement_b45a2* self, UnkActor* actor);

struct UnkSideMovementTuning_l9b62bc {
    u8 pad_0x0[0x168];
    bool turns;  // 0x168
    u8 pad_0x169[0xb];
    f32 max_angle_y;  // 0x174
    u8 pad_0x178[4];
    f32 rest_angle_z;  // 0x17c
};

struct UnkSideEnemyMovement_l9b62bc {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x38];
    f32 facing;  // 0x70
    u8 pad_0x74[0x1c20 - 0x74];
    UnkSideMovementTuning_l9b62bc* tuning;  // 0x1c20
};

struct UnkAngles_l9b62bc {
    f32 x;
    f32 y;
    f32 z;
};

static inline f32 Sign(f32 v) { return (f32)((v > 0.0f) - (v < 0.0f)); }

static inline bool NearZero(f32 v) { return v - 0.1f <= 0.0f && v + 0.1f >= 0.0f; }

// From `from` to `to` by `t`, held at the ends.
static inline f32 Lerp(f32 from, f32 to, f32 t) {
    if (t >= 1.0f)
        return to;
    if (t < 0.0f)
        return from;
    f32 rest = 1.0f - t;
    return t * to + rest * from;
}

void fn_71009b62bc(UnkSideEnemyMovement_l9b62bc* self, UnkAngles_l9b62bc* angles, f32 dt) {
    if (!self->tuning)
        return;
    UnkAnimationComponent* anim = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    if (anim) {
        if (fn_710117b348(anim, fn_7100bfc078()))
            return;
        if (self->tuning->turns && !fn_710117b348(anim, fn_7100d11fa8())) {
            f32 speed = fn_7100964f8c(reinterpret_cast<UnkEnemyMovement_b45a2*>(self->tuning), self->owner);
            f32 move = speed * dt;
            f32 step = move * Sign(self->facing);
            UnkSideMovementTuning_l9b62bc* tuning = self->tuning;

            f32 y = angles->y;
            f32 max_y = tuning->max_angle_y;
            if (fabsf(y) >= max_y) {
                y += move * (y < 0.0f ? 1.0f : -1.0f);
            } else {
                y += step;
                if (fabsf(y) >= max_y)
                    y = max_y * Sign(y);
            }

            f32 rest = tuning->rest_angle_z;
            f32 z = angles->z;
            if (NearZero(rest)) {
                f32 eased = Lerp(z, rest, speed * 0.5f * dt);
                angles->z = NearZero(eased) ? rest : eased;
            } else {
                z -= step;
                angles->z = fabsf(z) > rest ? rest * Sign(z) : z;
            }
            angles->y = y;
            angles->x = 0.0f;
            return;
        }
    }
    angles->y = 1.5707964f;
}
