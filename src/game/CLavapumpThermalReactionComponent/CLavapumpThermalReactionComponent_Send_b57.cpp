// CLavapumpThermalReactionComponent unit (tools/tu_map.py): reads the flag the global manager's blackboard
// (+0x968) section named by the string table's id at 0x1148 holds under the unit's property id
// (g_unk_7172759278, "COOLDOWNFINISHED", built on first use: game/UnkGameBlackboard.h) and, when it is
// set, sends the owner's (+0x30) sub-object at +0x220 a fixed hash with mode 0 (UnkSpitSend,
// src/game_s2/UnkSpitSend.h, written out so the flag keeps the top slot; the mode is a 4-byte local, so
// it shares the callee-save padding with the flag). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "types.h"

using base::global::CStrId;

extern UnkPropertyEx<> g_unk_7172759278;

struct UnkLavapumpOwner_b57 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkLavapumpSend_b57 {
    u8 pad_0x0[0x30];
    UnkLavapumpOwner_b57* owner;
};

void fn_7100a29890(UnkLavapumpSend_b57* self) {
    bool flag = false;
    CBlackboard* blackboard = &g_unk_7101d47310->field_0x968;
    const CStrId& id = base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x229];
    UnkQualifiedId& qid = ItemQualifiedId(g_unk_7172759278);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(g_unk_7172759278), g_unk_7101d470e0);
    if (CBlackboard::CSection* target = fn_7101047270(blackboard, id))
        fn_710006d11c(target, &qid.id, &flag);
    if (flag) {
        if (void* target = self->owner->field_0x220) {
            u32 mode = 0;
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(target, 0x1432c55a4f99ab04, 0xf4061ff25a2163d6,
                          reinterpret_cast<base::reflection::UnkReflTypeIdArg*>(&mode), &g_unk_710164b958,
                          &g_unk_7172769740, &mask);
        }
    }
}
