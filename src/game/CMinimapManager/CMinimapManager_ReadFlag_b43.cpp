// CMinimapManager::CTunableMinimapManager's unit (tools/tu_map.py): clears a flag far into the object
// (+0xd63d6a) and reads it from the game manager's blackboard: the section named by a global id, the
// value under the sub-id of the object's own name (+0x110) within a named definition (built on first
// use, as src/game/UnkSubareaSetupState.cpp's SubId). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

// As src/game/UnkSubareaSetupState.cpp and src/game_s7/fn_7100fc4b34.cpp declare it.
struct UnkSubId_7100fc4b34 {
    CStrId id;
    u64 field_0x8;
};
using UnkQualifiedName_7100fc4b34 = UnkQualifiedName;
const UnkSubId_7100fc4b34& fn_71003327a4(UnkQualifiedName_7100fc4b34* self, const CStrId* name);

struct UnkNamedDef_b43a3 : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedName name;
};
extern UnkNamedDef_b43a3 g_unk_71727c9f88;
extern CStrId g_unk_71727cc990;  // the blackboard section

struct UnkMinimapTunable_b43a3 {
    u8 pad_0x0[0x110];
    CStrId name;  // 0x110
    u8 pad_0x118[0xd63d6a - 0x118];
    bool flag;  // 0xd63d6a
};

static inline UnkSubId_7100fc4b34 SubId(UnkNamedDef_b43a3& def, const CStrId& name) {
    if (!def.name.str || !def.name.str[0])
        fn_7100ae9d7c(&def.name, &def, g_unk_7101d470e0);
    return fn_71003327a4(&def.name, &name);
}

__attribute__((always_inline)) inline void GetFlag(CBlackboard* set, const CStrId& section,
                                                   const UnkSubId_7100fc4b34& name, bool* out) {
    if (CBlackboard::CSection* s = fn_7101047270(set, section))
        fn_710006d11c(s, &name.id, out);
}

void fn_7100e99950(UnkMinimapTunable_b43a3* self) {
    CBlackboard* blackboard = &g_unk_7101d47310->field_0x968;
    self->flag = false;
    GetFlag(blackboard, g_unk_71727cc990, SubId(g_unk_71727c9f88, self->name), &self->flag);
}
