// CDredhedAIComponent unit (tools/tu_map.py): the hit handler. After the base handler, a hit from the
// side the owner faces is ignored; otherwise it is accepted (answer 2) unless the owner is frozen.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/CFrozenComponent.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkDredhedActor_l4e78b8 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x2c8 - 0x78];
    UnkFrostbiteComponent* frozen;  // 0x2c8
};

struct UnkDredhedSource_l4e78b8 {
    u8 pad_0x0[0x10];
    UnkDredhedActor_l4e78b8* actor;  // 0x10
};

struct UnkDredhedHit_l4e78b8 {
    u8 pad_0x0[0x8];
    UnkDredhedSource_l4e78b8* source;  // 0x8
    u8 pad_0x10[0xb4 - 0x10];
    u32 flags;  // 0xb4
};

struct UnkDredhedState_l4e78b8 {
    u8 pad_0x0[0x600];
    u32 field_0x600;
};

struct UnkDredhedAI_l4e78b8 {
    u8 pad_0x0[0x30];
    UnkDredhedActor_l4e78b8* owner;  // 0x30
    u8 pad_0x38[0x146c8 - 0x38];
    bool field_0x146c8;
    u8 pad_0x146c9[0x14730 - 0x146c9];
    UnkDredhedState_l4e78b8* state;  // 0x14730
};
static_assert(__builtin_offsetof(UnkDredhedAI_l4e78b8, state) == 0x14730, "");

static inline f32 Sign(f32 x) {
    return (f32)((x > 0.0f) - (x < 0.0f));
}

static inline bool InRange(f32 v, f32 lo, f32 hi) {
    return hi >= v && lo <= v;
}


static inline u32 Answer(UnkDredhedAI_l4e78b8* self, UnkDredhedHit_l4e78b8* hit) {
    UnkDredhedSource_l4e78b8* source = hit->source;
    if (!source || !source->actor)
        return 0;
    UnkDredhedActor_l4e78b8* owner = self->owner;
    f32 side = Sign(owner->transform.m[3] - source->actor->transform.m[3]);
    f32 facing = Sign(fn_710002614c(&owner->transform).m[2]);
    if (InRange(facing, side - 0.1f, side + 0.1f))
        return 0;
    if (hit->flags & 0xa00c4ee)
        return 0;
    UnkFrostbiteComponent* frozen = self->owner->frozen;
    if (!frozen)
        return 0;
    if (fn_71007f9f20(frozen))
        return 0;
    return (hit->flags & 0x40000) ? 0 : 2;
}

// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
u32 fn_71004e78b8(UnkDredhedAI_l4e78b8* self, UnkDredhedHit_l4e78b8* hit) {
    u32 result = fn_7101105e44(reinterpret_cast<UnkSluggerAI*>(self), reinterpret_cast<UnkSluggerEvent*>(hit));
    if (self->state->field_0x600 == 1) {
        if (self->field_0x146c8 && !(hit->flags & 0x2018260))
            return 2;
    } else if (!result) {
        return Answer(self, hit);
    }
    return result;
}