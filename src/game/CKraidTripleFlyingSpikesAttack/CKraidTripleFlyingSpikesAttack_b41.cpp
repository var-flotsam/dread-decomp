// CKraidTripleFlyingSpikesAttack unit. fn_71005f7f40: whether the attack can start: the base test
// (fn_71005f2014) holds, none of the three blocking tests (fn_71005f2024, fn_71005f2084,
// fn_71005f20e4) does, the owner's animation has no CCharClassCanStartLavaStreamAttackTrack, and,
// when the owner's AI is a CKraidAIComponent, it passes fn_71005ff8c0 and fails fn_71005ff90c and
// fn_71005ff92c. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkOwner_kt41 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkReflObject* ai;  // 0x240
};

struct UnkSelf_kt41 {
    u8 pad_0x0[0x8];
    UnkOwner_kt41* owner;
};

bool fn_71005f2014(UnkSelf_kt41* self, void* a, void* b);
bool fn_71005f2024(UnkSelf_kt41* self);
bool fn_71005f2084(UnkSelf_kt41* self);
bool fn_71005f20e4(UnkSelf_kt41* self);
bool fn_71005ff8c0(UnkReflObject* ai);
bool fn_71005ff90c(UnkReflObject* ai);
// Defined in src/game/CKraidAIComponent/CKraidAIComponent_b40.cpp without its (unused) argument.
bool fn_71005ff92c();

bool fn_71005f7f40(UnkSelf_kt41* self, void* a, void* b) {
    if (!fn_71005f2014(self, a, b) || fn_71005f2024(self) || fn_71005f2084(self) || fn_71005f20e4(self))
        return false;
    if (fn_710117b348(self->owner->animation, fn_7100c0e948()))
        return false;
    UnkReflObject* ai = self->owner->ai;
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_71005fac74()))
        return true;
    if (!fn_71005ff8c0(ai) || fn_71005ff90c(ai))
        return false;
    return !reinterpret_cast<bool (*)(UnkReflObject*)>(fn_71005ff92c)(ai);
}
