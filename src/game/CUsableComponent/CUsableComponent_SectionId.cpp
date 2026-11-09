// CUsableComponent's unit (tools/tu_map.py): the current scenario's blackboard section id, as fn_7100de9c30
// (src/game/CGameManager.cpp) gives it, inlined on the global game manager. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"

using base::global::CStrId;

CStrId fn_71011a2fbc(UnkGameManagerSub_0x608* scenario);

CStrId fn_7100a9f5e8() {
    UnkGameManager* manager = g_unk_7101d47310;
    if (manager->field_0x608)
        return fn_71011a2fbc(manager->field_0x608);
    return g_unk_7101d470e0;
}
