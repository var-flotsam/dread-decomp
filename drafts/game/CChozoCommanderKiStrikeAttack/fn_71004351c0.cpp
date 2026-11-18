// CChozoCommanderKiStrikeAttack's unit (tools/tu_map.py): whether the Ki strike may start. Without a
// Chozo Commander AI component, its character class or a target, the base check decides. Otherwise the
// base check must pass, the class's cooldown (+0x15b0) must have run out and both commander lists be
// empty; then the AI component's distances against the definition's and the attack's ranges decide.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkTU2702.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkHypersparkAttack3_ovb0023;
bool fn_710042bb38(UnkHypersparkAttack3_ovb0023* self);

struct UnkSentenceSphereEntry;
struct UnkSentenceSphereList {
    UnkSentenceSphereEntry** data;
    u32 count;
};
UnkSentenceSphereList* fn_710045bc94();

struct UnkCommanderEntry_ovb0073;
struct UnkCommanderList_ovb0073 {
    UnkCommanderEntry_ovb0073** data;
    u32 count;
};
UnkCommanderList_ovb0073* fn_7100459f94();

struct UnkKiStrikeClass_l4351c0 {
    u8 pad_0x0[0x15b0];
    f32 cooldown;  // 0x15b0
};

// The AI component's Ki strike state (+0x15d08).
struct UnkKiStrikeState_l4351c0 {
    f32 timer;  // 0x0
    u8 pad_0x4[0x1c4];
    bool near;  // 0x1c8
    bool side;  // 0x1c9
};

struct UnkKiStrikeAI_l4351c0 {
    u8 pad_0x0[0x10];
    UnkReflObject* char_class;  // 0x10
    u8 pad_0x18[0x15d08 - 0x18];
    UnkKiStrikeState_l4351c0 ki_strike;  // 0x15d08
};

struct UnkKiStrikeDef_l4351c0 {
    u8 pad_0x0[0x140];
    f32 range;  // 0x140
};

struct UnkKiStrike_l4351c0 {
    void* vtable;
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x40];
    UnkKiStrikeDef_l4351c0* def;  // 0x50
    u8 pad_0x58[0xc];
    f32 field_0x64;
    f32 field_0x68;
};

// The owner's AI component when it is a CChozoCommanderAIComponent.
static inline UnkKiStrikeAI_l4351c0* CommanderAI(UnkActor* owner) {
    UnkReflObject* obj = static_cast<UnkReflObject*>(owner->field_0x240);
    if (obj && fn_710009d50c(obj->vf_0x0(), fn_710044e708()))
        return reinterpret_cast<UnkKiStrikeAI_l4351c0*>(obj);
    return nullptr;
}

// The AI component's character class when it is a CCharClassChozoCommanderAIComponent.
static inline UnkKiStrikeClass_l4351c0* CharClass(UnkKiStrikeAI_l4351c0* ai) {
    UnkReflObject* obj = ai->char_class;
    if (obj && fn_710009d50c(obj->vf_0x0(), fn_710044d0c0()))
        return reinterpret_cast<UnkKiStrikeClass_l4351c0*>(obj);
    return nullptr;
}

bool fn_71004351c0(UnkKiStrike_l4351c0* self) {
    bool ok = fn_710042bb38(reinterpret_cast<UnkHypersparkAttack3_ovb0023*>(self));
    UnkKiStrikeAI_l4351c0* ai = CommanderAI(self->owner);
    UnkKiStrikeClass_l4351c0* cls = nullptr;
    UnkKiStrikeState_l4351c0* state = nullptr;
    if (ai) {
        cls = CharClass(ai);
        state = &ai->ki_strike;
    }
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!cls || !target)
        return ok;
    bool ready = false;
    if (ok && !(cls->cooldown > 0.0f && state->timer < cls->cooldown) && fn_710045bc94()->count == 0)
        ready = fn_7100459f94()->count == 0;
    f32 distance = 0.0f;
    fn_71010fc68c(reinterpret_cast<UnkGoliathAI_b58a3*>(ai), &distance);
    bool result = false;
    if (ready) {
        if (state->near) {
            result = !(distance < self->def->range + self->field_0x64);
        } else {
            f32 reach = 0.0f;
            fn_71010fc778(ai, &reach);
            if (!(reach + 10.0f < self->field_0x68))
                result = true;
            else if (state->side)
                result = distance - __builtin_fabsf(target->field_0x44 - self->owner->field_0x44) + 10.0f >=
                         self->field_0x68;
        }
    }
    return result;
}
