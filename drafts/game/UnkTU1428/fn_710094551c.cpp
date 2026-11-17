// CMorphBallMovement's unit: the ball's update after the base one. Picks the step for the frame
// (fn_710093fab8 when nearly still or the sub-area manager says so, else the ground or air step),
// counts down the timers at +0x23d0 and +0x2648, runs the mode's own step (in mode 3 the spin part's
// position and angle and the interpolator's time) and scales the rolling sound by the speed.
// Names and layouts are placeholders.
#include <math.h>

#include "base/global/CRntVector.h"
#include "game/CFXComponent.h"
#include "game/UnkTU0.h"
#include "game/UnkTU101.h"
#include "game/UnkTU2577.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkSamusMovementUpdate;
struct UnkMorphBallMovement_l949b7c;
struct UnkContact_ovb0048;
struct UnkBallMovement_l946fb0;

void fn_71010dae00(UnkSamusMovementUpdate* self, f32 dt);  // the base update
UnkVec3f fn_7100949b7c(UnkMorphBallMovement_l949b7c* self);
UnkVec3f fn_71010d252c(bool flag, const UnkContact_ovb0048* contact);
void fn_7100946fb0(UnkBallMovement_l946fb0* self);
bool fn_7100eef684(UnkManager* manager);

// A part of the owner's model: placed by position and angle.
struct UnkFxPart_551c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void SetPosition(const UnkVec3f* position);
    virtual void SetAngle(const UnkVec3f* angle);
};

struct UnkBallOwner_551c {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x280 - 0x94];
    UnkWorld* world;  // 0x280
};
static_assert(__builtin_offsetof(UnkBallOwner_551c, world) == 0x280, "layout");

struct UnkBallPartDef_551c {
    u8 pad_0x0[0x30];
    u64 part_hash;  // 0x30
};

struct UnkBallInterp_551c {
    u8 pad_0x0[0x44];
    f32 time;  // 0x44
};

struct UnkBallInterpRef_551c {
    UnkBallInterp_551c* object;
    u64 id;
};

struct UnkBallSoundDef_551c {
    u8 pad_0x0[0x10];
    f32 volume;  // 0x10
};

struct UnkBallSound_551c {
    u8 pad_0x0[0x8];
    UnkBallSoundDef_551c* def;
    u8 pad_0x10[0xc];
    f32 volume;  // 0x1c
};

struct UnkMorphBallMovement_551c {
    u8 pad_0x0[0x30];
    UnkBallOwner_551c* owner;  // 0x30
    u8 pad_0x38[0x84 - 0x38];
    UnkVec3f target;  // 0x84
    u8 pad_0x90[0xf9 - 0x90];
    bool on_ground;  // 0xf9
    u8 pad_0xfa[0x118 - 0xfa];
    f32 speed;  // 0x118
    u8 pad_0x11c[0x128 - 0x11c];
    u8 contact[0x15c];  // 0x128
    UnkXform xform;     // 0x284
    u8 pad_0x2e0[0x396 - 0x2e0];
    bool field_0x396;
    u8 pad_0x397[0x2288 - 0x397];
    s32 mode;  // 0x2288
    u8 pad_0x228c[0x2340 - 0x228c];
    UnkBallInterpRef_551c* interp;  // 0x2340
    u64 interp_id;                   // 0x2348
    u8 pad_0x2350[0x23d0 - 0x2350];
    f32 timer;  // 0x23d0
    u8 pad_0x23d4[0x2489 - 0x23d4];
    bool timer_done;  // 0x2489
    u8 pad_0x248a[2];
    UnkVec3f up;  // 0x248c
    u8 pad_0x2498[0x24a4 - 0x2498];
    f32 field_0x24a4;
    u8 pad_0x24a8[0x2530 - 0x24a8];
    f32 field_0x2530;
    u8 pad_0x2534[0x2550 - 0x2534];
    UnkVec3f field_0x2550;
    u8 pad_0x255c[0x2588 - 0x255c];
    UnkBallPartDef_551c* part_def;  // 0x2588
    u8 pad_0x2590[0x25ec - 0x2590];
    bool field_0x25ec;
    u8 pad_0x25ed[0x2628 - 0x25ed];
    UnkBallSound_551c* sound;  // 0x2628
    u8 pad_0x2630[0x2644 - 0x2630];
    bool field_0x2644;
    u8 pad_0x2645[3];
    f32 field_0x2648;
};
static_assert(__builtin_offsetof(UnkMorphBallMovement_551c, xform) == 0x284, "layout");
static_assert(__builtin_offsetof(UnkMorphBallMovement_551c, interp) == 0x2340, "layout");
static_assert(__builtin_offsetof(UnkMorphBallMovement_551c, field_0x2648) == 0x2648, "layout");

void fn_710093fab8(UnkMorphBallMovement_551c* self, f32 dt, bool still);
void fn_71009459b8(UnkMorphBallMovement_551c* self, f32 dt);
void fn_710094646c(UnkMorphBallMovement_551c* self);
void fn_7100945d00(UnkMorphBallMovement_551c* self, f32 dt);
void fn_7100946d58(UnkMorphBallMovement_551c* self, f32 dt);
void fn_7100946a9c(UnkMorphBallMovement_551c* self);
void fn_7100946804(UnkMorphBallMovement_551c* self);

static inline f32 Clamp_551c(f32 v, f32 lo, f32 hi) {
    return v <= lo ? lo : (v >= hi ? hi : v);
}

// The cross product, written component by component into the result.
inline UnkVec3f Cross_551c(const UnkVec3f& a, const UnkVec3f& b) {
    UnkVec3f r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}

// Turns the part about z so that it faces along `up`.
inline void FaceAlong(UnkFxPart_551c* part, const UnkVec3f& up) {
    f32 angle = atan2f(1.0f, 0.0f) - atan2f(up.y, -up.x);
    if (angle < 0.0f)
        angle += 6.283185307179586;
    UnkVec3f rotation(0.0f, 0.0f, angle);
    part->SetAngle(&rotation);
}

// The ball's sideways direction for its mode.
inline UnkVec3f SideDirection(UnkMorphBallMovement_551c* self) {
    switch (self->mode) {
    case 1:
        return fn_71010d252c(self->on_ground, reinterpret_cast<const UnkContact_ovb0048*>(self->contact));
    case 2:
    case 4:
    case 5:
        return Cross_551c(fn_7100949b7c(reinterpret_cast<UnkMorphBallMovement_l949b7c*>(self)),
                          UnkVec3f(0.0f, 0.0f, 1.0f));
    case 3:
        return Cross_551c(self->field_0x2550, UnkVec3f(0.0f, 0.0f, 1.0f));
    default:
        return UnkVec3f(1.0f, 0.0f, 0.0f);
    }
}

void fn_710094551c(UnkMorphBallMovement_551c* self, f32 dt) {
    fn_71010dae00(reinterpret_cast<UnkSamusMovementUpdate*>(self), dt);
    self->field_0x396 = false;

    // Not used further, in the original too.
    UnkVec3f side = SideDirection(self);
    (void)side;

    if (fabsf(self->field_0x24a4) >= 0.1f) {
        UnkVec3f from = self->xform.GetTranslation();
        UnkVec3f to = self->target;
        UnkManager* manager = g_unk_7101d47360;
        if (!manager) {
            manager = static_cast<UnkManager*>(fn_71000839a0(0xae8, 0x3f));
            fn_7100ee1f34(manager);
            g_unk_7101d47360 = manager;
        }
        if (fn_7100eef684(manager)) {
            fn_710093fab8(self, dt, false);
        } else if (self->on_ground) {
            fn_71009459b8(self, dt);
        } else if (DistanceSq(to, from) >= dt * 100.0f) {
            fn_7100945d00(self, dt);
        } else {
            fn_710094646c(self);
        }
    } else {
        fn_710093fab8(self, dt, true);
    }

    if (self->timer > 0.0f) {
        self->timer -= dt;
        if (self->timer < 0.0f)
            self->timer_done = true;
    }
    if (self->field_0x25ec && LengthSq(self->up) > 0.001f)
        self->field_0x25ec = false;

    switch (self->mode) {
    case 2:
    case 5:
        fn_7100946d58(self, dt);
        fn_7100946a9c(self);
        break;
    case 4:
        fn_7100946804(self);
        if (self->mode == 4)
            fn_7100946a9c(self);
        break;
    case 3: {
        UnkBallOwner_551c* owner = self->owner;
        UnkVec3f position = owner->xform.GetTranslation();
        UnkFxPart_551c* part =
            reinterpret_cast<UnkFxPart_551c*>(fn_7101152b90(owner->world, self->part_def->part_hash));
        if (part) {
            part->SetPosition(&position);
            FaceAlong(part, self->up);
        }
        UnkBallInterpRef_551c* ref = self->interp;
        if (ref && ref->id == self->interp_id && self->interp_id != ~0ull && ref->object) {
            fn_7100105da8(reinterpret_cast<UnkInterp_7100105d34*>(ref->object),
                          Clamp_551c(ref->object->time + dt, 0.1f, 1.5f), 0.2f);
        }
    }
    // fall through
    default:
        fn_7100946fb0(reinterpret_cast<UnkBallMovement_l946fb0*>(self));
        break;
    }

    if (UnkBallSound_551c* sound = self->sound) {
        f32 scale = Clamp_551c((self->speed - 1.0f) / 499.0f, 0.0f, 1.0f);
        sound->volume = sound->def->volume * scale;
    }

    if (self->field_0x2648 >= 0.0f) {
        self->field_0x2648 -= dt;
        if (self->field_0x2648 < 0.0f && self->field_0x2644) {
            self->field_0x2644 = false;
            self->field_0x2530 = 0.0f;
            self->field_0x2648 = -1.0f;
        }
    }
}
