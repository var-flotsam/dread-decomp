// GUI::CEndingRewardsComposition's unit (tools/tu_map.py unit 1673): the reveal of a secret entry, as fn_7100af79a0 in
// src/game/GUI/CChozoArchivesComposition/CChozoArchivesComposition_Reveal_ovb0124.cpp does it, with this unit's
// "SecretVisited" property (g_unk_717275aef8). Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1645.h"
#include "game/UnkTU2553.h"
#include "game/UnkTUProperty.h"
#include "types.h"
#include "game/UnkTypedValue.h"

using base::global::CStrId;

struct UnkProfileSaveArg {
    void* field_0x0;
    void* field_0x8;
};

static inline const CStrId& IdTableKey(u32 offset) {
    return *reinterpret_cast<const CStrId*>(GameManagerIdTable().pad_0x0 + offset);
}

// The property's qualified id (game/UnkGameBlackboard.h), built on first use.
static inline const UnkQualifiedId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = reinterpret_cast<UnkQualifiedId&>(item.field_0x10);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, reinterpret_cast<UnkIdSource*>(&item), g_unk_7101d470e0);
    return qid;
}

struct UnkEndingRewardsItem_ovb0265 {
    void** vtable;

    void vf_0x1e8(const char* name, bool b) {
        reinterpret_cast<void (*)(UnkEndingRewardsItem_ovb0265*, const char*, bool)>(vtable[0x1e8 / 8])(this, name, b);
    }
};

// Stores `value` under `name` in the section `section` names (unless the blackboard is locked).
static inline void SetBool_ovb0265(CBlackboard* blackboard, const CStrId& section, const UnkQualifiedId& name,
                                   const bool& value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = const_cast<bool*>(&value);
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(const_cast<UnkQualifiedId*>(&name)), &v);
        }
    }
}

void fn_7100b1d890(void* self, UnkEndingRewardsItem_ovb0265* item) {
    UnkGameManager* gm = g_unk_7101d47310;
    CBlackboard* profile = reinterpret_cast<CBlackboard*>(reinterpret_cast<char*>(gm) + 0x820);
    bool visited = false;
    {
        const CStrId& section = IdTableKey(0x1be8);
        const UnkQualifiedId& name = Qid(g_unk_717275aef8);
        if (CBlackboard::CSection* s = fn_7101047270(profile, section))
            fn_710006d11c(s, &name.id, &visited);
    }
    if (visited)
        return;
    SetBool_ovb0265(profile, IdTableKey(0x1be8), Qid(g_unk_717275aef8), true);
    UnkProfileSaveArg arg = {nullptr, nullptr};
    fn_7100df99f8(g_unk_7101d47310, 0, &arg, 0);
    item->vf_0x1e8("SecretMemory_Revealed", false);
}
