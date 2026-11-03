// Unit 1444 (tools/tu_map.py, no reflected name): whether the component may act now: it is enabled
// (+0x1fe), not blocked (+0x202), not forced (+0x200), the owner's character class (the animation
// component at +0x220 of the owner at +0x30) has neither a CCharClassCanBeAbortedTrack nor a
// CCharClassAllowAttackTrack, and the owner's AI component answers no to its slot 0x5b8 query.
// Names and layout are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"


struct UnkUnit1444Component_b53 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x1c6];
    bool field_0x1fe;
    u8 pad_0x1ff;
    bool field_0x200;
    u8 pad_0x201;
    bool field_0x202;
};

bool fn_71009745f0(UnkUnit1444Component_b53* self) {
    if (!self->field_0x1fe || self->field_0x202)
        return false;
    if (self->field_0x200)
        return false;
    auto* cls = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    if (!cls)
        return false;
    if (fn_710117b348(cls, fn_7100c0f988()))
        return false;
    if (fn_710117b348(static_cast<UnkAnimationComponent*>(self->owner->field_0x220), fn_7100bfb148()))
        return false;
    void** ai = static_cast<void**>(self->owner->field_0x240);
    if (!ai)
        return false;
    auto query = reinterpret_cast<bool (*)(void*, void*)>((*reinterpret_cast<void***>(ai))[0x5b8 / 8]);
    return !query(ai, nullptr);
}
