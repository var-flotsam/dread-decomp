// CCoreXAIComponent unit (tools/tu_map.py unit 759): steers the movement towards the target at the
// AI's speed, turning by at most the turn limit once the turn delay has passed, and slows down along
// each axis near the bounds of the area; it also fires the lock-on missiles on a timer through the
// owner's timeline. Names and layouts are placeholders.
#include <math.h>

#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCharacterMovement.h"
#include "game/CMovementComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU53.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;
using base::reflection::UnkReflObject;

struct UnkBossAI_l3ae15c;
void fn_71003ae15c(UnkBossAI_l3ae15c* self, f32 dt);
UnkVec3f fn_710007a390(const UnkVec3f& v, f32 angle);

extern u64 g_unk_7101649bd8;  // the timeline id of the missile volley

struct UnkCoreXStaticZero_l4da0a0 {
    u32 value;
    UnkCoreXStaticZero_l4da0a0() : value(0) {}
};

inline const UnkCoreXStaticZero_l4da0a0& UnkCoreXStaticZero() {
    static UnkCoreXStaticZero_l4da0a0 s_value asm("g_unk_7101d57cc8");
    return s_value;
}

inline u32 UnkCoreXDefaultMask() {
    static u32 s_mask asm("g_unk_7101d57cb8") = ~UnkCoreXStaticZero().value & 0xff;
    return s_mask;
}

// A component slot: the reflected component at +0x10.
struct UnkCoreXSlot_l4da0a0 {
    u8 pad_0x0[0x10];
    UnkReflObject* component;  // 0x10
};

// The registered movements fn_7100aba0a8 lists.
struct UnkCoreXSlotList_l4da0a0 {
    UnkCoreXSlot_l4da0a0** items;
    u32 count;
};
UnkCoreXSlotList_l4da0a0* fn_7100aba0a8();

struct UnkCoreXLife_l4da0a0 {
    UnkCoreXSlot_l4da0a0 slot;
    u8 pad_0x18[0x58];
    s32 state;  // 0x70
};

struct UnkCoreXCurve_l4da0a0 {
    u8 pad_0x0[0x10];
    const char* modifiers;  // 0x10
    u8 pad_0x18[0x2c];
    s32 uses;  // 0x44
};

struct UnkCoreXOwner_l4da0a0;

struct UnkCoreXParams_l4da0a0 {
    void** vtable;
    u8 pad_0x8[0x51c];
    f32 accel;          // 0x524
    f32 turn_accel;     // 0x528
    f32 max_turn;       // 0x52c, degrees
    f32 slow_distance;  // 0x530
    f32 slow_speed;     // 0x534
    UnkCoreXCurve_l4da0a0* slow_curve;  // 0x538
    u8 pad_0x540[0x4];
    f32 turn_delay;     // 0x544
    f32 volley_time;    // 0x548

    f32 vf_0xb0(UnkCoreXOwner_l4da0a0* owner) {
        return reinterpret_cast<f32 (*)(UnkCoreXParams_l4da0a0*, UnkCoreXOwner_l4da0a0*)>(vtable[0xb0 / 8])(this,
                                                                                                         owner);
    }
};
static_assert(__builtin_offsetof(UnkCoreXParams_l4da0a0, volley_time) == 0x548, "layout");

struct UnkCoreXMovement_l4da0a0 {
    u8 pad_0x0[0x1d8];
    UnkVec3f velocity;  // 0x1d8
};

struct UnkCoreXOwner_l4da0a0 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x168];
    UnkCoreXLife_l4da0a0* life;  // 0x1d0
    u8 pad_0x1d8[0x40];
    UnkCoreXMovement_l4da0a0* movement;  // 0x218
    u8 pad_0x220[0x60];
    void* field_0x280;
    u8 pad_0x288[0x60];
    UnkTimelineComponent* timeline;  // 0x2e8
};
static_assert(__builtin_offsetof(UnkCoreXOwner_l4da0a0, timeline) == 0x2e8, "layout");

struct UnkCoreXSteer_l4da0a0 {
    UnkCoreXParams_l4da0a0* params;  // 0x0
    CVector2D min;  // 0x8
    CVector2D max;  // 0x10
    u8 pad_0x18[0xc];
    s32 state;      // 0x24: 1 waiting to fire, 2 firing
    f32 timer;      // 0x28
    f32 cooldown;   // 0x2c
};

struct UnkCoreXAI_l4da0a0 {
    u8 pad_0x0[0x30];
    UnkCoreXOwner_l4da0a0* owner;  // 0x30
    u8 pad_0x38[0x14968];
    UnkCoreXSteer_l4da0a0 steer;  // 0x149a0
};
static_assert(__builtin_offsetof(UnkCoreXAI_l4da0a0, steer) == 0x149a0, "layout");

static inline bool IsEnemyLife(UnkReflObject* obj) {
    return obj && fn_710009d50c(obj->vf_0x0(), fn_71008b3b04());
}

static inline bool IsLockOnMissile(UnkReflObject* obj) {
    return obj && fn_710009d50c(obj->vf_0x0(), fn_7100ab5d80());
}

static inline f32 Clamp(f32 v, f32 lo, f32 hi) {
    return v <= lo ? lo : (!(v >= hi) ? v : hi);
}

static inline UnkVec3f Negated(const UnkVec3f& v) {
    return UnkVec3f(-v.x, -v.y, -v.z);
}

static inline UnkVec3f Cross(const UnkVec3f& a, const UnkVec3f& b) {
    return UnkVec3f(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

static inline f32 Dot(const UnkVec3f& a, const UnkVec3f& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline f32 InvLength(const UnkVec3f& v) {
    return 1.0f / (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z) + 0.0001f);
}

// The speed allowed at `distance` from the bound: the AI's speed far from it, down to the slow speed
// at it, along the slow curve.
static inline f32 SpeedNearBound(UnkCoreXParams_l4da0a0* params, f32 speed, f32 radius, f32 distance) {
    f32 t = (distance - radius) / ((radius + params->slow_distance) - radius);
    ++params->slow_curve->uses;
    const char* modifiers = params->slow_curve->modifiers ? params->slow_curve->modifiers : g_unk_710162e2ed;
    f32 w = fn_7100076020(1.0f - Clamp(t, 0.0f, 1.0f), modifiers);
    f32 slow = params->slow_speed;
    if (w >= 1.0f)
        return slow;
    if (w < 0.0f)
        return speed;
    return speed * (1.0f - w) + w * slow;
}

void fn_71004da0a0(UnkCoreXAI_l4da0a0* self, f32 dt) {
    UnkCoreXMovement_l4da0a0* movement = self->owner->movement;
    UnkCoreXLife_l4da0a0* life = self->owner->life;
    bool enemy = life && IsEnemyLife(life->slot.component);
    if (movement == nullptr || life == nullptr || !enemy)
        return;
    if (!self->owner->field_0x280)
        return;
    UnkTimelineComponent* timeline = self->owner->timeline;
    if (!timeline || life->state == 2)
        return;

    fn_71003ae15c(reinterpret_cast<UnkBossAI_l3ae15c*>(self), dt);
    UnkCoreXSteer_l4da0a0* s = &self->steer;
    if (s->cooldown > 0.0f)
        s->cooldown -= dt;
    UnkCoreXParams_l4da0a0* params = s->params;
    bool can_turn = params->max_turn > 0.0f;
    s->timer += dt;
    bool turn_ready = !(s->timer < params->turn_delay);
    switch (s->state) {
    case 1:
        if (s->timer >= 0.2f) {
            UnkCoreXSlotList_l4da0a0* list = fn_7100aba0a8();
            bool found = false;
            for (UnkCoreXSlot_l4da0a0 **it = list->items, **end = it + list->count; it != end; ++it) {
                if (*it && IsLockOnMissile((*it)->component)) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                alignas(8) u32 mask = UnkCoreXDefaultMask();
                fn_7100a2cb7c(timeline, &g_unk_7101649bd8, &mask, false, -1.0f);
                s->state = 2;
            }
        }
        break;
    case 2:
        if (s->timer >= s->params->volley_time) {
            u64 id = 0x35870cc1063a1052;
            if (fn_7100a2be94(timeline, &id))
                fn_7100a2b7b8(timeline, &g_unk_7101649bd8);
            s->state = 0;
        }
        break;
    }

    f32 speed = s->params->vf_0xb0(self->owner);
    UnkVec3f velocity = movement->velocity;
    f32 inv = InvLength(velocity);
    UnkVec3f heading(velocity.x * inv, velocity.y * inv, velocity.z * inv);

    UnkActor* target = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
    UnkCoreXOwner_l4da0a0* owner = self->owner;
    UnkVec3f to(target->field_0x44 - owner->x, target->field_0x54 - owner->y, target->field_0x64 - owner->z);
    inv = InvLength(to);
    UnkVec3f wanted(speed * (to.x * inv), speed * (to.y * inv), speed * (to.z * inv));
    UnkVec3f change(wanted.x - velocity.x, wanted.y - velocity.y, wanted.z - velocity.z);
    inv = InvLength(change);
    UnkVec3f dir(inv * change.x, inv * change.y, inv * change.z);

    f32 accel = s->params->accel;
    if (can_turn && turn_ready) {
        f32 angle = acosf(Clamp(Dot(dir, heading), -1.0f, 1.0f));
        f32 max_turn = s->params->max_turn * 3.141592653589793 / 180.0;
        if (angle > max_turn) {
            const UnkVec3f up(0.0f, 0.0f, 1.0f);
            f32 turn = Dot(dir, Cross(heading, Negated(up))) < 0.0f ? -max_turn : max_turn;
            UnkVec3f turned = fn_710007a390(heading, turn);
            accel = s->params->turn_accel;
            dir.x = turned.x;
            dir.y = turned.y;
            dir.z = 0.0f;
        }
    }

    UnkVec3f next(velocity.x + dir.x * accel * dt, velocity.y + dir.y * accel * dt,
                  velocity.z + dir.z * accel * dt);
    f32 radius = fn_7100977b20(movement);

    s32 sign_x = (next.x > 0.0f) - (next.x < 0.0f);
    f32 fsign_x = sign_x;
    CVector2D bound = sign_x > 0 ? s->max : s->min;
    f32 limit = SpeedNearBound(s->params, speed, radius, fabsf(bound.x - self->owner->x));
    f32 vx = next.x * fsign_x;
    next.x = (vx < limit ? vx : limit) * fsign_x;

    s32 sign_y = (next.y > 0.0f) - (next.y < 0.0f);
    f32 fsign_y = sign_y;
    bound = sign_y > 0 ? s->max : s->min;
    limit = SpeedNearBound(s->params, speed, radius, fabsf(bound.y - self->owner->y));
    f32 vy = next.y * fsign_y;
    next.y = (vy < limit ? vy : limit) * fsign_y;

    next.z = 0.0f;
    fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(movement),
                  reinterpret_cast<const UnkMovementVec_71010d08d4*>(&next), true);
}
