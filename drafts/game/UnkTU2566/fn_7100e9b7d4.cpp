// CMinimapManager::CTunableMinimapManager's unit (tools/tu_map.py): whether the game manager's blackboard has a key, or
// else holds true under the sub-id of the given name in the minimap section. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2566.h"
#include "game/UnkSubId_7100fc4b34.h"
#include "types.h"

using base::global::CStrId;

struct UnkNamedDef_le9b7d4 : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedName name;
};
extern UnkNamedDef_le9b7d4 g_unk_71727c9c78;

static inline UnkSubId_7100fc4b34 SubId(UnkNamedDef_le9b7d4& def, const CStrId& name) {
    if (!def.name.str || !def.name.str[0])
        fn_7100ae9d7c(&def.name, &def, g_unk_7101d470e0);
    return fn_71003327a4(&def.name, &name);
}

// Reads the named flag into `out`; false when the section is missing.
static inline bool GetFlag(CBlackboard* set, const CStrId& section, const UnkSubId_7100fc4b34& name, bool* out) {
    CBlackboard::CSection* s = fn_7101047270(set, section);
    if (!s)
        return false;
    return fn_710006d11c(s, &name.id, out);
}

bool fn_7100e9b7d4(void* self, const CStrId* key) {
    CBlackboard* blackboard = &g_unk_7101d47310->field_0x968;
    if (fn_7101047208(blackboard, *key))
        return true;
    bool value = false;
    UnkSubId_7100fc4b34 sub = SubId(g_unk_71727c9c78, *key);
    return GetFlag(blackboard, g_unk_71727cc990, sub, &value) & value;
}
