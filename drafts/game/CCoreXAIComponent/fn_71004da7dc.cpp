// CCoreXAIComponent's unit (tools/tu_map.py unit 759): the damage handler. After the boss handler, a
// hit pushes the core back (away from its target or along the hit's normal) and, unless it is lethal
// or the drop is cooling down, throws a fan of drops from in front of it. Names and layouts are
// placeholders.
#include "game/CBossAIComponent.h"
#include "game/CCoreXAIComponent.h"
#include "game/CMovementComponent.h"
#include "game/UnkActor.h"
#include "game/UnkVec3fOps.h"
#include "types.h"

extern "C" int rand();

// As src/game/UnkTU2702/UnkTU2702_Misc_ovb0143.cpp declares it: throws one drop.
void fn_71007c3a38(char* actor, void* a, void* b, void* c, u32 d, bool e);
// As src/game/CKraidBouncingCreaturesAttack/CKraidBouncingCreaturesAttack_Launch_l5f31e4.cpp declares it.
UnkVec3f RotateVec3AboutZ(const UnkVec3f& v, f32 angle);

// The tuning the component reads.
struct UnkCoreXSettings_l4da7dc {
    void** vtable;
    u8 pad_0x8[0x540 - 0x8];
    f32 push;  // 0x540
    u8 pad_0x544[0x8];
    f32 drop_cooldown;  // 0x54c
    s32 min_drops;      // 0x550
    s32 max_drops;      // 0x554

    f32 MaxSpeed(UnkActor* owner) {
        return reinterpret_cast<f32 (*)(UnkCoreXSettings_l4da7dc*, UnkActor*)>(vtable[0xb0 / 8])(this, owner);
    }
};
static_assert(__builtin_offsetof(UnkCoreXSettings_l4da7dc, max_drops) == 0x554, "layout");

struct UnkCoreXHit_l4da7dc {
    u8 pad_0x0[0x78];
    UnkVec3f direction;  // 0x78
    UnkVec3f normal;     // 0x84
    u8 pad_0x90[0xb4 - 0x90];
    u32 flags;  // 0xb4
    u8 pad_0xb8[0xf8 - 0xb8];
    f32 damage;  // 0xf8
};
static_assert(__builtin_offsetof(UnkCoreXHit_l4da7dc, damage) == 0xf8, "layout");

struct UnkCoreXLife_l4da7dc {
    u8 pad_0x0[0x54];
    f32 value;  // 0x54
};

struct UnkCoreXMovement_l4da7dc {
    u8 pad_0x0[0x1d8];
    UnkVec3f velocity;  // 0x1d8
};

struct UnkCoreXAI_l4da7dc {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x149a0 - 0x38];
    UnkCoreXSettings_l4da7dc* settings;  // 0x149a0
    u8 pad_0x149a8[0x1c];
    s32 state;          // 0x149c4
    f32 field_0x149c8;
    f32 drop_cooldown;  // 0x149cc
};
static_assert(__builtin_offsetof(UnkCoreXAI_l4da7dc, drop_cooldown) == 0x149cc, "layout");

static inline UnkVec3f Pos(const UnkActor* actor) {
    return UnkVec3f(actor->field_0x44, actor->field_0x54, actor->field_0x64);
}

// A random number from `lo` to `hi`, both included.
static inline s32 RandomInRange(s32 lo, s32 hi) {
    s32 end = hi + 1;
    if (end == lo)
        return lo;
    return (rand() & 0x7fff) % (end - lo) + lo;
}

// `v` clamped to [0, 1].
inline f32 Clamp01(f32 v) {
    return !(v <= 0.0f) ? (!(v >= 1.0f) ? v : 1.0f) : 0.0f;
}

// A random number from 0 to 1.
static inline f32 Random01() {
    return (rand() & 0x7fff) * (1.0 / 32767.0);
}

static inline void Push(UnkCoreXMovement_l4da7dc* movement, const UnkVec3f* impulse) {
    fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(movement),
                  reinterpret_cast<const UnkMovementVec_71010d08d4*>(impulse), true);
}

// Adds `impulse` to the movement's current velocity.
inline void AddPush(UnkCoreXMovement_l4da7dc* movement, const UnkVec3f& impulse) {
    UnkVec3f total = impulse + movement->velocity;
    Push(movement, &total);
}

u32 fn_71004da7dc(UnkCoreXAI_l4da7dc* self, UnkCoreXHit_l4da7dc* hit) {
    if (u32 r = fn_71003adaf8(reinterpret_cast<UnkQuetzoaXAI_b58*>(self),
                              reinterpret_cast<UnkQuetzoaXMessage_b58*>(hit)))
        return r;
    u32 flags = hit->flags;
    if (flags & 0x8000)
        return 0;

    UnkVec3f dir = hit->direction;
    f32 distance;
    u32 result;
    bool drop;
    if (flags & 0x2a00045e) {
        f32 push = self->settings->push;
        dir = -hit->normal;
        if (flags & 0x200) {
            push *= 0.1f;
            distance = 112.0f;
        } else {
            if ((flags & 0x2800040) && fn_7100d7c990(self->owner)) {
                UnkActor* owner = self->owner;
                dir = Normalized(Pos(owner) - Pos(fn_7100d7c990(owner)));
                push *= 0.8f;
            }
            distance = 160.0f;
        }
        result = 2;
        drop = true;
        if (self->state != 2) {
            self->field_0x149c8 = 0.0f;
            UnkCoreXMovement_l4da7dc* movement =
                static_cast<UnkCoreXMovement_l4da7dc*>(self->owner->field_0x218);
            if (movement) {
                UnkVec3f impulse(push * dir.x, push * dir.y, push * dir.z);
                if (self->state != 1) {
                    Push(movement, &impulse);
                } else if (hit->flags & 0x200) {
                    AddPush(movement, impulse);
                }
            }
            self->state = 1;
            result = 0;
        }
    } else {
        distance = 0.0f;
        result = 2;
        drop = false;
        if (flags & 0x205001) {
            drop = true;
            UnkCoreXMovement_l4da7dc* movement =
                static_cast<UnkCoreXMovement_l4da7dc*>(self->owner->field_0x218);
            if (movement) {
                UnkVec3f velocity = movement->velocity;
                UnkVec3f heading = Normalized(velocity);
                f32 ratio = Length(velocity) / self->settings->MaxSpeed(self->owner);
                f32 t = (1.0f - Clamp01(ratio)) * Clamp01(Dot(heading, dir) + 1.0f);
                if (t >= 1.0f)
                    distance = 160.0f;
                else if (t < 0.0f)
                    distance = 50.0f;
                else
                    distance = (1.0f - t) * 50.0f + t * 160.0f;
            }
        }
    }

    UnkActor* owner = self->owner;
    UnkCoreXLife_l4da7dc* life = static_cast<UnkCoreXLife_l4da7dc*>(owner->field_0x1b8);
    if (life && hit->damage >= life->value)
        return result;
    if (!drop || !(self->drop_cooldown <= 0.0f))
        return result;
    self->drop_cooldown = self->settings->drop_cooldown;
    char* dropper = fn_71004dad10(owner);
    if (!dropper)
        return result;

    UnkCoreXSettings_l4da7dc* settings = self->settings;
    s32 count = RandomInRange(settings->min_drops, settings->max_drops);
    u32 parity = rand() & 1;
    if (count > 0) {
        // The drops fan out over a quarter turn centred on `dir`.
        f32 step = 1.5707964f / (f32)(count - 1);
        for (s32 i = 0; i != count; ++i) {
            // The first drops (one or two) are of the other kind.
            u32 kind = i <= parity;
            UnkVec3f out = RotateVec3AboutZ(dir, -0.7853982f + step * (f32)i);
            f32 speed = Random01() * 300.0f + 300.0f;
            UnkVec3f velocity = out * speed;
            UnkActor* from = self->owner;
            UnkVec3f position(from->field_0x44 + distance * dir.x, from->field_0x54 + distance * dir.y,
                              from->field_0x64 + distance * dir.z);
            fn_71007c3a38(dropper, nullptr, &position, &velocity, kind, false);
        }
    }
    return result;
}
