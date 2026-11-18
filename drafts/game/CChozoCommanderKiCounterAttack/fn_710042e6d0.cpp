// CChozoCommanderKiCounterAttack's unit (tools/tu_map.py): whether the attack may start. When the owner
// is a Chozo Commander with its character class and a target, the commander must be ready, healthy enough,
// and the target neither attacking nor too close; then no sentence sphere may be out.
// Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CMeleeComponent.h"
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

struct UnkKiCounterTunables_l42e6d0 {
    u8 pad_0x0[0xf4];
    f32 field_0xf4;  // the reach the target must keep
};

struct UnkKiCounter_l42e6d0 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x40];
    UnkKiCounterTunables_l42e6d0* field_0x50;
};

struct UnkCommanderClass_l42e6d0 : UnkReflObject {
    u8 pad_0x8[0x15a8];
    f32 field_0x15b0;  // the health threshold
    u8 pad_0x15b4[0x8];
    f32 field_0x15bc;  // the height margin
};
static_assert(__builtin_offsetof(UnkCommanderClass_l42e6d0, field_0x15bc) == 0x15bc, "layout");

struct UnkCommanderAI_l42e6d0 : UnkReflObject {
    u8 pad_0x8[0x8];
    UnkReflObject* field_0x10;  // the character class component
    u8 pad_0x18[0x15cee];
    bool field_0x15d06;  // ready
    u8 pad_0x15d07;
    f32 field_0x15d08;
    f32 field_0x15d0c;
    u8 pad_0x15d10[0x1c0];
    bool field_0x15ed0;
};
static_assert(__builtin_offsetof(UnkCommanderAI_l42e6d0, field_0x15d08) == 0x15d08, "layout");
static_assert(__builtin_offsetof(UnkCommanderAI_l42e6d0, field_0x15ed0) == 0x15ed0, "layout");

// The actor slots this unit reads beyond UnkActor's.
struct UnkActorSlots_l42e6d0 {
    u8 pad_0x0[0x2f8];
    UnkMeleeComponent* field_0x2f8;  // the melee component
};

static inline UnkActorSlots_l42e6d0* Slots_l42e6d0(UnkActor* actor) {
    return reinterpret_cast<UnkActorSlots_l42e6d0*>(actor);
}

// Whether the target leaves room for the counter: it is not attacking, and it is out of reach or
// below the commander.
static inline bool TargetAllows(UnkKiCounter_l42e6d0* self, UnkCommanderAI_l42e6d0* commander,
                                UnkCommanderClass_l42e6d0* cls, UnkActor* target) {
    UnkMeleeComponent* melee = Slots_l42e6d0(target)->field_0x2f8;
    bool allowed = !(melee && fn_710092b604(melee));
    if (allowed && !commander->field_0x15ed0 && self->field_0x50->field_0xf4 > 0.0f) {
        f32 vertical = 0.0f;
        f32 horizontal = 0.0f;
        fn_71010fc778(commander, &vertical);
        fn_71010fc39c(commander, &horizontal);
        f32 reach = self->field_0x50->field_0xf4;
        if (vertical < reach &&
            (__builtin_fabsf(horizontal - target->field_0x44) < reach ||
             !(target->field_0x54 < self->owner->field_0x54 + cls->field_0x15bc)))
            allowed = false;
    }
    return allowed;
}

bool fn_710042e6d0(UnkKiCounter_l42e6d0* self) {
    if (!fn_710042bb38(reinterpret_cast<UnkHypersparkAttack3_ovb0023*>(self)))
        return false;

    auto* component = static_cast<UnkReflObject*>(self->owner->field_0x240);
    auto* commander = component && fn_710009d50c(component->vf_0x0(), fn_710044e708())
                          ? static_cast<UnkCommanderAI_l42e6d0*>(component)
                          : nullptr;
    UnkCommanderClass_l42e6d0* cls = nullptr;
    if (commander) {
        UnkReflObject* charClass = commander->field_0x10;
        cls = charClass && fn_710009d50c(charClass->vf_0x0(), fn_710044d0c0())
                  ? static_cast<UnkCommanderClass_l42e6d0*>(charClass)
                  : nullptr;
    }
    UnkActor* target = fn_7100d7c990(self->owner);
    if (commander && cls && target) {
        if (!commander->field_0x15d06)
            return false;
        f32 threshold = cls->field_0x15b0;
        if (threshold > 0.0f &&
            (commander->field_0x15d08 < threshold * 0.5f || commander->field_0x15d0c < threshold))
            return false;
        if (!TargetAllows(self, commander, cls, target))
            return false;
    }
    return fn_710045bc94()->count == 0;
}
