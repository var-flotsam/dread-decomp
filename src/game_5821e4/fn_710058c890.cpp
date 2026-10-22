// Static initialiser of CGoliathXBurstProjectionBombMovement's unit.
#include "UnkStaticInit.h"

using namespace unk_static_init;

struct UnkStatics_710058c890 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    UnkFilePathStrIdBase field_0x28;
    UnkFilePathStrIdBase field_0x30;
    u8 pad_0x38[0x28];
    u8 field_0x60[0x10];
    u8 field_0x70[8];
};
extern UnkStatics_710058c890 g_unk_71726e5cc0 __attribute__((visibility("hidden")));
void* fn_7100589df4();  // CGoliathXBurstProjectionBombMovement::refl_GetClass
extern char g_unk_71726e5ca8[];
extern UnkFilePathStrIdBase g_unk_71726e5cb0;
extern UnkFilePathStrIdBase g_unk_71726e5cb8;
void fn_710058a2b8(void* self);

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
