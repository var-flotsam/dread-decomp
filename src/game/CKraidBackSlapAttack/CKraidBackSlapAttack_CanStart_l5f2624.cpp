// CKraidBackSlapAttack's unit (tools/tu_map.py unit 913). fn_71005f2624: whether the back slap can
// start: the base test holds and, when the owner's AI is a CKraidAIComponent with a target in mode 1,
// its attacker list does not hold too many other attacks before the back slap. Names and layouts are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CKraidAIComponent.h"
#include "game/CKraidAttack.h"
#include "game/UnkActor.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkKraidAI_ovb0118;

// Entries of the Kraid AI component's attacker list.
struct UnkKraidAttackNode_l5f2624 {
    u8 unk_0x0[0x8];
    void* attack;  // 0x8
    u8 unk_0x10[0x8];
    UnkKraidAttackNode_l5f2624* next;  // 0x18
};

struct UnkKraidAI_l5f2624 : UnkReflObject {
    u8 unk_0x8[0x1138];
    s32 mode;  // 0x1140
    u8 unk_0x1144[0x13994];
    u8 grab_attack[0xa8];   // 0x14ad8
    u8 slap_attack[0x40];   // 0x14b80
    u8 other_attack[0x40];  // 0x14bc0
};
static_assert(__builtin_offsetof(UnkKraidAI_l5f2624, grab_attack) == 0x14ad8, "");
static_assert(__builtin_offsetof(UnkKraidAI_l5f2624, slap_attack) == 0x14b80, "");
static_assert(__builtin_offsetof(UnkKraidAI_l5f2624, other_attack) == 0x14bc0, "");

struct UnkAttackList_l5f2624 {
    u8 unk_0x0[0x10];
    UnkKraidAttackNode_l5f2624* head;  // 0x10
};

struct UnkAttackContext_l5f2624 {
    u8 unk_0x0[0x20];
    UnkAttackList_l5f2624* list;  // 0x20
};

struct UnkKraidBackSlapAttack_l5f2624 {
    u8 unk_0x0[0x8];
    UnkActor* owner;  // 0x8
};

bool fn_71005f2624(UnkKraidBackSlapAttack_l5f2624* self, UnkAttackContext_l5f2624* context, void* b) {
    UnkSelf_kt41* base = reinterpret_cast<UnkSelf_kt41*>(self);
    if (!fn_71005f2014(base, context, b))
        return false;
    UnkKraidAI_l5f2624* ai = static_cast<UnkKraidAI_l5f2624*>(self->owner->field_0x240);
    int ok = true;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_71005fac74()) && fn_7100d7c990(self->owner) &&
        ai->mode == 1) {
        ok = fn_71005f20e4(base) && fn_71005fdb70(reinterpret_cast<UnkKraidAI_ovb0118*>(ai)) &&
             !fn_71005ff8c0(ai);
        if (ok) {
            bool grab_seen = false;
            u32 others = 0;
            for (UnkKraidAttackNode_l5f2624* node = context->list->head; node; node = node->next) {
                if (node->attack == ai->slap_attack) {
                    ok = ok & (grab_seen | (others < 2));
                    break;
                }
                if (node->attack == ai->other_attack)
                    break;
                if (node->attack == ai->grab_attack)
                    grab_seen = true;
                else
                    others++;
            }
        }
    }
    return ok;
}
