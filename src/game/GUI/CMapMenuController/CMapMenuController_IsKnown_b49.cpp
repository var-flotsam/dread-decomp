// GUI::CMapMenuController's unit (tools/tu_map.py, the name from reflection): whether the game
// manager's blackboard knows the id at +0x0: fn_71010474f4 in the section g_unk_71727cc990 under the
// sub-id of that id within a named definition (built on first use), as
// src/game/CMinimapManager/CMinimapManager_ReadFlag_b43.cpp reads its flag. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkMapNamedDef_b49.h"
#include "game/UnkTU2566.h"
#include "game/UnkSubId_7100fc4b34.h"
#include "types.h"

using base::global::CStrId;

static inline UnkSubId_7100fc4b34 SubId(UnkMapNamedDef_b49& def, const CStrId& name) {
    if (!def.name.str || !def.name.str[0])
        fn_7100ae9d7c(&def.name, &def, base::global::CStrId::Empty());
    return fn_71003327a4(&def.name, &name);
}

bool fn_7100b37a80(const CStrId* id) {
    CBlackboard* blackboard = &g_unk_7101d47310->field_0x968;
    return fn_71010474f4(blackboard, g_unk_71727cc990, SubId(g_unk_71727c9e00, *id).id);
}
