// CScorpiusTailSmashAttack: when the owner's AI component is a CScorpiusAIComponent whose value at
// +0x14cd4 exceeds the sum of the two values at +0x5a0 and +0x5a4 of its character class, asks the
// component (fn_7100696088). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkScorpiusObject {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkScorpiusObject*)>(vtable[0])(this);
    }
};

struct UnkScorpiusCharClass : UnkScorpiusObject {
    u8 pad_0x8[0x598];
    f32 field_0x5a0;
    f32 field_0x5a4;
};

struct UnkScorpiusAI : UnkScorpiusObject {
    u8 pad_0x8[0x8];
    UnkScorpiusObject* char_class;  // 0x10
    u8 pad_0x18[0x14cbc];
    f32 field_0x14cd4;
};

struct UnkTailSmashAttack {
    u8 pad_0x0[0x8];
    UnkActor* owner;
};

// Called with the value, the limit and the second term (still in s0-s2; it may not read them).
bool fn_7100696088(UnkScorpiusAI* ai, f32 value, f32 limit, f32 extra);

bool fn_710068c284(UnkTailSmashAttack* self) {
    UnkScorpiusAI* ai = static_cast<UnkScorpiusAI*>(self->owner->field_0x240);
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_7100690c74()))
        return false;
    UnkScorpiusObject* object = ai->char_class;
    UnkScorpiusCharClass* cc =
        fn_710009d50c(object->vf_0x0(), fn_710068f63c()) ? static_cast<UnkScorpiusCharClass*>(object) : nullptr;
    f32 value = ai->field_0x14cd4;
    f32 base = cc->field_0x5a0;
    f32 extra = cc->field_0x5a4;
    f32 limit = base + extra;
    if (value > limit)
        return fn_7100696088(ai, value, limit, extra);
    return false;
}
