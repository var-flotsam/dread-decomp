// CEscapeSequenceExplosionComponent unit (tools/tu_map.py): the game manager's list of the objects
// registered under the string id of an explosion kind. Kinds 0, 1 and 2 name the unit's three static
// ids; any other kind keeps "SmallExplosion". Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

extern CStrId g_unk_717271e2d0 __attribute__((visibility("hidden")));
extern CStrId g_unk_717271e2d8 __attribute__((visibility("hidden")));
extern CStrId g_unk_717271e2e0 __attribute__((visibility("hidden")));

UnkGameManagerIdList* fn_71007d5d8c(s32 kind) {
    CStrId id("SmallExplosion", true);
    switch (kind) {
    case 0:
        id = g_unk_717271e2d0;
        break;
    case 1:
        id = g_unk_717271e2d8;
        break;
    case 2:
        id = g_unk_717271e2e0;
        break;
    }
    return fn_7100de94cc(g_unk_7101d47310, reinterpret_cast<const u64*>(&id));
}
