// UnkTU1428 (CMorphBallMovement's unit): the ball's speed along a direction for one step: the boost
// wears off, the input accelerates or brakes it, and the result is held to the state's top speed.
// Names and layouts are placeholders.
#include "game/CAbilitySpeedBooster.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkPlayer;
struct UnkAbility;
struct UnkGroundMovement_ovb0138;

bool fn_71010d15ac(UnkGroundMovement_ovb0138* movement);
f32 fn_71010d6858();

// The speed booster's tunable (registered elsewhere): only the member read here.
struct UnkSpeedBoosterTunable_l9439dc {
    u8 pad_0x0[0x34];
    f32 speed_factor;  // 0x34
};

// CMorphBallMovement's tunable (src/reflection/CMorphBallMovement.cpp): only the member read here.
struct UnkMorphBallTunable_l9439dc {
    u8 pad_0x0[0x5c];
    f32 field_0x5c;
};

extern UnkSpeedBoosterTunable_l9439dc g_unk_71726b0fc0;
extern UnkMorphBallTunable_l9439dc g_unk_71727393d8;

// The slow-motion flags as bytes of the game manager's word at +0xc4.
struct UnkSlomoFlags_l9439dc {
    bool enabled;  // 0xc4
    u8 unk_0xc5;
    bool paused;  // 0xc6
};

struct UnkVec3_l9439dc {
    f32 x, y, z;
};

struct UnkBoost_l9439dc {
    f32 time;
    UnkVec3_l9439dc dir;
};

struct UnkRollConfig_l9439dc {
    u8 pad_0x0[0xc8];
    f32 boost_decay;  // 0xc8
    u8 pad_0xcc[4];
    f32 brake;  // 0xd0
    f32 accel;  // 0xd4
};

struct UnkRollSurface_l9439dc {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkRollMovement_l9439dc {
    void** vtable;
    u8 pad_0x8[0x40 - 0x8];
    UnkVec3_l9439dc input;  // 0x40
    u8 pad_0x4c[0x7c - 0x4c];
    f32 field_0x7c;
    u8 pad_0x80[0xf9 - 0x80];
    bool field_0xf9;
    u8 pad_0xfa[0x128 - 0xfa];
    UnkRollSurface_l9439dc* field_0x128;
    u8 pad_0x130[0x1d8 - 0x130];
    UnkVec3_l9439dc velocity;  // 0x1d8
    u8 pad_0x1e4[0x3a8 - 0x1e4];
    bool field_0x3a8;
    u8 pad_0x3a9[3];
    f32 field_0x3ac;
    u8 pad_0x3b0[0x7e4 - 0x3b0];
    bool field_0x7e4;
    u8 pad_0x7e5[0x2280 - 0x7e5];
    UnkRollConfig_l9439dc* config;  // 0x2280
    s32 state;                       // 0x2288
    f32 field_0x228c;
    f32 field_0x2290;
    f32 field_0x2294;
    f32 field_0x2298;
    u8 pad_0x229c[0x2540 - 0x229c];
    bool field_0x2540;
    u8 pad_0x2541[3];
    f32 field_0x2544;
    u8 pad_0x2548[0x2634 - 0x2548];
    UnkBoost_l9439dc boost;  // 0x2634
    u8 pad_0x2644[0x2658 - 0x2644];
    f32 field_0x2658;
};

static inline f32 Dot(const UnkVec3_l9439dc& a, const UnkVec3_l9439dc& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline f32 Sign(f32 x) {
    return (f32)((x > 0.0f) - (x < 0.0f));
}

static inline bool IsNear(f32 x, f32 epsilon) {
    return x + epsilon >= 0.0f && x - epsilon <= 0.0f;
}

static inline bool IsRolling(s32 state) {
    return state == 2 || state == 4 || state == 5;
}

static inline f32 SpeedFactor() {
    return g_unk_71726b0fc0.speed_factor;
}

// Turning against the speed: the speed left is the part the grip keeps, the other way.
static inline bool Brake(UnkRollMovement_l9439dc* self, f32 speed, f32 brake, f32* v) {
    f32 grip = self->field_0xf9 ? brake : 0.0f;
    f32 input = self->input.x;
    if (speed < 0.0f && input > 0.0f) {
        *v = __builtin_fabsf(speed * grip);
        return true;
    }
    if (speed > 0.0f && input < 0.0f) {
        *v = -__builtin_fabsf(speed * grip);
        return true;
    }
    return false;
}

static inline bool Boosted() {
    UnkAbility* ability = fn_7100df5928(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310));
    return ability && fn_710033cf20(ability);
}

void fn_71009439dc(UnkRollMovement_l9439dc* self, const UnkVec3_l9439dc* dir, f32 dt) {
    if (self->boost.time > 0.0f) {
        self->boost.time -= self->config->boost_decay * dt;
        if (self->boost.time < 0.0f || Dot(self->velocity, self->boost.dir) < 0.001f)
            self->boost = UnkBoost_l9439dc();
    }

    bool moving = self->field_0x7c > 0.0f ||
                  (IsRolling(self->state) && self->field_0x2544 > 0.0f && !self->field_0x2540) ||
                  (fn_71010d15ac(reinterpret_cast<UnkGroundMovement_ovb0138*>(self)) &&
                   reinterpret_cast<bool (*)(UnkRollMovement_l9439dc*)>(self->vtable[0x3b0 / 8])(self));
    if (!moving)
        return;

    UnkRollConfig_l9439dc* config = self->config;
    f32 speed = Dot(*dir, self->velocity);
    f32 brake = config->brake;
    f32 accel = config->accel;
    bool boosted = Boosted();
    f32 v = boosted ? speed / SpeedFactor() : speed;

    s32 state = self->state;
    f32 target;
    if (state == 3) {
        target = self->field_0x2294;
    } else {
        if (IsRolling(state) && self->field_0x2544 > 0.0f)
            Brake(self, speed, brake, &v);
        else if (self->field_0x2658 == 0.0f && Brake(self, speed, brake, &v))
            self->boost = UnkBoost_l9439dc();
        target = v + accel * self->input.x * dt;
    }
    if (boosted)
        target *= SpeedFactor();

    f32 top;
    switch (state) {
    case 1:
        top = self->field_0x228c;
        break;
    case 2:
    case 4:
    case 5:
        top = self->field_0x2290;
        break;
    case 3:
        top = self->field_0x2294;
        break;
    default:
        top = 0.0f;
        break;
    }
    UnkGameManager* manager = g_unk_7101d47310;
    if (manager->field_0xc8 != 0 || manager->field_0x10c) {
        UnkSlomoFlags_l9439dc* slomo = reinterpret_cast<UnkSlomoFlags_l9439dc*>(&manager->field_0xc4);
        if (slomo->enabled && !slomo->paused)
            top *= fn_71010d6858();
    }
    if (Boosted())
        top *= SpeedFactor();

    if (self->state == 3) {
        top = self->field_0x2294;
        if (boosted)
            top *= SpeedFactor();
    } else if (!boosted) {
        UnkRollSurface_l9439dc* surface = self->field_0x128;
        if (self->field_0xf9 || !surface || !surface->field_0x10 ||
            (!self->field_0x3a8 && (self->field_0x3ac != 0.0f || (IsNear(self->input.x, 0.001f) && IsNear(self->input.y, 0.001f) &&
                                                                  IsNear(self->input.z, 0.001f)))))
            top = self->field_0x2298;
    }

    f32 limit = self->field_0x7e4
                    ? top * g_unk_71727393d8.field_0x5c
                    : top;
    f32 result;
    if (self->field_0x2658 > 0.0f && !IsNear(speed, 0.1f) && Sign(target) == Sign(speed) &&
        __builtin_fabsf(target) >= __builtin_fabsf(speed))
        result = speed;
    else
        result = target <= -limit ? -limit : (target >= limit ? limit : target);

    f32 delta = result + self->boost.time * Sign(result) - speed;
    self->velocity.x += dir->x * delta;
    self->velocity.y += dir->y * delta;
    self->velocity.z += dir->z * delta;
}
