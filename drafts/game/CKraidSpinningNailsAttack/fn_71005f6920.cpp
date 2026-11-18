// CKraidSpinningNailsAttack's unit (tools/tu_map.py unit 918): whether the attack can start. The base
// test (fn_71005f2014) must hold; with a CKraidAIComponent in mode 1 and a target, the attack must
// pass the component's tests, not be its current attack, and come in the attack list before the
// component's own attack at +0x14b80 either after the one at +0x14ad8 or with fewer than two others
// before it. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CKraidAIComponent.h"
#include "game/CKraidAttack.h"
#include "game/UnkActor.h"
#include "game/UnkTU2702.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkKraidAI_ovb0118;
struct UnkScorpiusAI_b69a4;
bool fn_71005fdb70(UnkKraidAI_ovb0118* ai);

struct UnkKraidAI_l5f6920 : UnkReflObject {
    u8 pad_0x8[0x1138];
    u32 mode;  // 0x1140
    u8 pad_0x1144[0x14ad8 - 0x1144];
    u8 attack_14ad8[0xa8];  // the component's own attacks
    u8 attack_14b80[0x40];
    u8 attack_14bc0[0x40];
};
static_assert(__builtin_offsetof(UnkKraidAI_l5f6920, attack_14bc0) == 0x14bc0, "UnkKraidAI_l5f6920 layout");

struct UnkKraidAttackNode_l5f6920 {
    u8 pad_0x0[0x8];
    void* attack;  // 0x8
    u8 pad_0x10[0x8];
    UnkKraidAttackNode_l5f6920* next;  // 0x18
};

struct UnkKraidAttackList_l5f6920 {
    u8 pad_0x0[0x10];
    UnkKraidAttackNode_l5f6920* head;  // 0x10
};

struct UnkKraidAttackContext_l5f6920 {
    u8 pad_0x0[0x20];
    UnkKraidAttackList_l5f6920* attacks;  // 0x20
};

struct UnkKraidNailsAttack_l5f6920 {
    void* vtable;
    UnkActor* owner;  // 0x8
};

bool fn_71005f6920(UnkKraidNailsAttack_l5f6920* self, UnkKraidAttackContext_l5f6920* ctx, void* b) {
    UnkSelf_kt41* base = reinterpret_cast<UnkSelf_kt41*>(self);
    if (!fn_71005f2014(base, ctx, b))
        return false;
    auto* ai = static_cast<UnkKraidAI_l5f6920*>(self->owner->field_0x240);
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_71005fac74()) || !fn_7100d7c990(self->owner) || ai->mode != 1)
        return true;
    if (!fn_71005f20e4(base) || !fn_71005fdb70(reinterpret_cast<UnkKraidAI_ovb0118*>(ai)) || fn_71005ff8c0(ai))
        return false;
    // Not while it is the component's current attack.
    bool ready = fn_7101105970(static_cast<UnkScorpiusAI_b69a4*>(self->owner->field_0x240)) != self;
    if (ready) {
        u32 others = 0;
        bool after_first = false;
        for (UnkKraidAttackNode_l5f6920* node = ctx->attacks->head; node; node = node->next) {
            void* attack = node->attack;
            if (attack == ai->attack_14b80) {
                ready = after_first || others < 2;
                break;
            }
            if (attack == ai->attack_14bc0)
                break;
            after_first |= attack == ai->attack_14ad8;
            if (attack != ai->attack_14ad8)
                ++others;
        }
    }
    return ready;
}
