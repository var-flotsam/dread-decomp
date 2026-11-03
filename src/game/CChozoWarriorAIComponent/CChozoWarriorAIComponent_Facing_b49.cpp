// CChozoWarriorAIComponent (tools/tu_map.py): whether the warrior should turn. False without its
// component at +0x218 or the actor it acts against; true unless that component is in state 1; in
// state 1 only while +0x149cc is 1, and then when its facing (the transform's +0x8) points away from
// the target (signs of the facing and of the x offset differ) or is not yet decided (|facing| < 0.9).
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkActor.h"
#include "types.h"

u32 fn_71010cc3a4(void*);

struct UnkChozoWarriorAI_b49 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x149cc - 0x38];
    s32 field_0x149cc;
};

static inline f32 Sign(f32 v) {
    return (f32)((s32)(v > 0.0f) - (s32)(v < 0.0f));
}

static inline f32 Facing(UnkActor* actor) {
    return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38)).m[2];
}

bool fn_710049c1a8(UnkChozoWarriorAI_b49* self) {
    void* component = self->owner->field_0x218;
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!component || !target)
        return false;
    if (fn_71010cc3a4(component) != 1)
        return true;
    if (self->field_0x149cc != 1)
        return false;
    f32 facing = Sign(Facing(self->owner));
    UnkActor* owner = self->owner;
    f32 offset = target->field_0x44 - owner->field_0x44;
    const UnkMatrix44* transform = reinterpret_cast<const UnkMatrix44*>(owner->field_0x38);
    f32 side = Sign(offset);
    return __builtin_fabsf(fn_710002614c(transform).m[2]) < 0.9f || side != facing;
}
