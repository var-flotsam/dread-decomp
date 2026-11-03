// CChozoCommanderXLifeComponent's static initialiser: the unit header, seven names (actor, model,
// event and the boss-defeated log entry), the component's id, one bool property and the class's
// registration.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"

// The unit's data (its statics, merged into one block).
struct UnkChozoCommanderXLifeTU {
    UnkTUHeader header;
    CStrId names[7];  // 0x18
    u8 field_0x50[0x18];
    UnkReflType type;  // 0x68
};

extern UnkChozoCommanderXLifeTU g_unk_7172730280 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172730208;
extern UnkProperty g_unk_7172730210;
extern char g_unk_7101a7e120[];
void fn_71008bfd70(UnkProperty* p);
void fn_71008bd078(void* p);
extern char g_unk_7172730238;

static const char* const kNames[7] = {
    "ChozoCommanderX", "ChozoCommanderX", "Impact", "bigx_commanderx", "boss",
    "Event log when Boss is defeated", "DefeatedBossID",
};

void fn_71008c0270() {
    UnkChozoCommanderXLifeTU& tu = g_unk_7172730280;
    InitHeader(tu.header);
    for (int i = 0; i < 7; ++i) {
        new (&tu.names[i]) CStrId(kNames[i], true);
        fn_7100000250(fn_710000051c, &tu.names[i], &g_unk_7101a19000);
    }
    new (&g_unk_7172730208) CStrId("CHOZOCOMMANDERX", true);
    fn_7100000250(fn_710000051c, &g_unk_7172730208, &g_unk_7101a19000);
    InitPropertyVtableFirst(&g_unk_7172730210, reinterpret_cast<void**>(g_unk_7101a7e120), g_unk_71726db788, "Dead",
                            refl_TypeOf<fn_710000d228>(), fn_71008bfd70, fn_71008bd078);
    fn_71008bd0f4();
    fn_710009ec00(&g_unk_7172730238, &tu.type);
}
