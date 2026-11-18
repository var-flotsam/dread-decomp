// CKraidFierceSwipeAttack's unit (unit 915): whether the attack can start: the base test (fn_71005f2014)
// holds, and when the owner's AI is a CKraidAIComponent with a target, the blocking test (fn_71005f20e4)
// also holds, neither of the AI's own two attacks is due next in the request's queue (the first at the
// head, the second within the first two), and fn_71005ff8c0 does not hold. Names and layouts are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CKraidAIComponent.h"
#include "game/CKraidAttack.h"
#include "game/UnkActor.h"
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

// The Kraid AI component's two attacks of its own.
struct UnkKraidAI_l5f44f0 {
    u8 pad_0x0[0x14b80];
    u8 attack_0x14b80[0x40];
    u8 attack_0x14bc0[0x40];
};

struct UnkAttackNode_l5f44f0 {
    u8 pad_0x0[0x8];
    void* attack;  // 0x8
    u8 pad_0x10[0x8];
    UnkAttackNode_l5f44f0* next;  // 0x18
};

struct UnkAttackQueue_l5f44f0 {
    u8 pad_0x0[0x10];
    UnkAttackNode_l5f44f0* head;  // 0x10
};

struct UnkAttackRequest_l5f44f0 {
    u8 pad_0x0[0x20];
    UnkAttackQueue_l5f44f0* queue;  // 0x20
};

bool fn_71005f44f0(UnkSelf_kt41* self, void* a, void* b) {
    if (!fn_71005f2014(self, a, b))
        return false;
    UnkReflObject* ai = self->owner->ai;
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_71005fac74()) ||
        !fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner)))
        return true;
    if (!fn_71005f20e4(self))
        return false;
    UnkKraidAI_l5f44f0* kraid = reinterpret_cast<UnkKraidAI_l5f44f0*>(ai);
    u32 index = 0;
    bool second_found = false;
    for (UnkAttackNode_l5f44f0* node = static_cast<UnkAttackRequest_l5f44f0*>(a)->queue->head;
         node && !second_found; node = node->next) {
        if (node->attack == kraid->attack_0x14b80) {
            if (index == 0)
                return false;
            break;
        }
        second_found = node->attack == kraid->attack_0x14bc0;
        if (!second_found)
            ++index;
    }
    if (second_found && index < 2)
        return false;
    return !fn_71005ff8c0(ai);
}
