// Static initialiser of CGoliathXBurstProjectionBombMovement's unit.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CGoliathXBurstProjectionBombMovement.h"
#include "game/UnkStatics_710058c890.h"

using namespace unk_static_init;

extern char g_unk_71726e5ca8[];

void fn_710058c890() {
    InitHeaderIds(&g_unk_71726e5cc0.ids);
    fn_7100589df4();
    fn_710009ec00(g_unk_71726e5ca8, g_unk_71726e5cc0.field_0x70);
    InitPathId(&g_unk_71726e5cb0, "actors/weapons/supergoliathlinebomb/fx/bomb.bcptl");
    InitPathId(&g_unk_71726e5cb8, "actors/weapons/supergoliathlinebomb/fx/tracker.bcptl");
    InitId(&g_unk_71726e5cc0.field_0x20, "fDamageSuperGoliathBurstProjectionTracker");
    InitPathId(&g_unk_71726e5cc0.field_0x28, "actors/weapons/supergoliathlinebomb/fx/explosion.bcptl");
    InitPathId(&g_unk_71726e5cc0.field_0x30, "actors/weapons/supergoliathlinebomb/fx/explosion_tracker.bcptl");
    {
        UnkStrIdTmp ids[] = {UnkStrIdTmp("default", true), UnkStrIdTmp("trespass", true)};
        fn_710058a070(g_unk_71726e5cc0.field_0x60, ids, 2);
    }
    __cxa_atexit(fn_710058a2b8, g_unk_71726e5cc0.field_0x60, g_unk_7101a19000);
}
