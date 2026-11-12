// CAttack's unit (unit 444 of tools/tu_map.py, the name from reflection): with a target object (+0x20), passes it
// to fn_71003646b0 with `value`, replaced by the AI component's value (+0x1788) when the component's string id
// (+0x1780) is set and names the same string (hash at +0x30 of the string data) as the target's id (+0x38).
// Reading an id's hash bumps a counter in its string data (+0x44). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CCharClassAttack.h"
#include "types.h"

struct UnkIdData_A2_b68 {
    u8 pad_0x0[0x30];
    u64 hash;
    u8 pad_0x38[0x44 - 0x38];
    union {
        u32 use_count;
    };
};

struct UnkAttackTarget_A2_b68 {
    u8 pad_0x0[0x38];
    union {
        UnkIdData_A2_b68* id;
    };
};

struct UnkAttackAI_A2_b68 {
    u8 pad_0x0[0x1780];
    union {
        UnkIdData_A2_b68* id;
    };
    u32 value;
};

struct UnkAttackActor_A2_b68 {
    u8 pad_0x0[0x240];
    UnkAttackAI_A2_b68* ai;
};

struct UnkAttack_A2_b68 {
    u8 pad_0x0[0x8];
    UnkAttackActor_A2_b68* actor;
    u8 pad_0x10[0x10];
    UnkAttackTarget_A2_b68* target;
};

void* fn_7100362324(UnkAttack_A2_b68* self, u32 value) {
    UnkAttackTarget_A2_b68* first = self->target;
    if (!first)
        return nullptr;
    if (UnkAttackActor_A2_b68* actor = self->actor) {
        if (UnkAttackAI_A2_b68* ai = actor->ai) {
            if (reinterpret_cast<void*>(ai->id) != &g_unk_7101d47108->empty) {
                ++ai->id->use_count;
                UnkIdData_A2_b68* id = ai->id;
                UnkAttackTarget_A2_b68* target = self->target;
                ++target->id->use_count;
                bool same = id->hash == target->id->hash;
                first = self->target;
                if (same)
                    value = ai->value;
            }
        }
    }
    return fn_71003646b0(first, value);
}
