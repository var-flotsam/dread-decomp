// CCharClassSetSceneGroupEnabledEvent unit (tools/tu_map.py): unless the scene group id at +0x10 is the
// empty string id, asks the render manager to enable or disable it (fn_7100219c6c with the flag at +0x18
// and the key at +0x30 of the id's data). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU315.h"
#include "types.h"

struct UnkSceneGroupData_b47a3 {
    u8 unk_0x0[0x30];
    u64 key;
};

struct UnkSetSceneGroupEnabledEvent_b47a3 {
    u8 unk_0x0[0x10];
    UnkSceneGroupData_b47a3* group;
    bool enabled;
};

void fn_7100bdeb38(UnkSetSceneGroupEnabledEvent_b47a3* self) {
    UnkSceneGroupData_b47a3* group = self->group;
    if (reinterpret_cast<void*>(group) == &g_unk_7101d47108->empty)
        return;
    fn_7100219c6c(g_unk_7101d47300, self->enabled, group->key, 0, false);
}
