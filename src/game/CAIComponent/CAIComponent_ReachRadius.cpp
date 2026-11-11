// CAIComponent unit: a reach radius. The tracked object's reach (fn_7100977b90, as
// src/game/CAIComponent.cpp declares it), its radius overridden by the tunable's value at +0xf8 when
// that is not negative, halved; the tunable's value at +0xf4 wins when it is not negative.
// Names and layouts are placeholders.
#include "game/CCharacterMovement.h"
#include "types.h"

struct UnkAIReach {
    f32 radius;
    f32 field_0x4;

    UnkAIReach() : radius(0.0f), field_0x4(0.0f) {}
    UnkAIReach(const UnkAIReach& other);
};

struct UnkAIReachOwner {
    u8 unk_0x0[0x1f8];
    void* tracked;
};

struct UnkAIReachTunable {
    u8 unk_0x0[0xf4];
    f32 radius;
    f32 reach;
};

struct UnkAIReachComponent {
    u8 unk_0x0[0x30];
    UnkAIReachOwner* owner;
    u8 unk_0x38[0x1610 - 0x38];
    UnkAIReachTunable* tunable;
};

// `value` unless it is negative (or not a number), else `fallback`.
static inline f32 PickNonNegative(f32 fallback, f32 value) {
    if (value >= 0.0f)
        fallback = value;
    return fallback;
}

f32 fn_71010faa5c(UnkAIReachComponent* self) {
    UnkAIReach reach = fn_7100977b90(self->owner->tracked);
    UnkAIReachTunable* t = self->tunable;
    if (t->reach >= 0.0f)
        reach.radius = t->reach;
    return PickNonNegative(reach.radius * 0.5f, t->radius);
}
