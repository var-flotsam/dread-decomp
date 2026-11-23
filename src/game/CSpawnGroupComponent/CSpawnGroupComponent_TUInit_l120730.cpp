// CSpawnGroupComponent's static initialiser (tools/tu_map.py unit 2703): the unit header, the
// component's id, its "Enabled" and "NumDeaths" properties, its registration, the default spawn
// points value and the two ids the game manager lists the groups under. Names and layouts are
// placeholders.
#include "base/UnkStrIdRegistry.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CSpawnGroupComponent.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"

// The unit's data (its statics, merged into one block).
struct UnkSpawnGroupTU {
    UnkTUHeader header;
    u64 field_0x18;
    CStrId metroid_key;  // 0x20
    CStrId key;          // 0x28
    UnkReflType type;    // 0x30
};

extern UnkSpawnGroupTU g_unk_7172a06798 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172a065d8;
extern char g_unk_7172a06630;
extern UnkValue g_unk_7172a06638;
extern void* g_unk_7101cb2828[];
void fn_710111fa80(UnkProperty* p);
void fn_71011173fc(void* p);

// The component's id lives in an inline function: its guard and object are reached through the GOT.
inline const CStrId& SpawnGroupId() {
    static CStrId s_id("SPAWNGROUP", true);
    return s_id;
}

void fn_7101120730() {
    UnkSpawnGroupTU& tu = g_unk_7172a06798;
    InitHeader(tu.header);
    new (&g_unk_7172a065d8) CStrId("SPAWNGROUP", true);
    fn_7100000250(CStrId_Destroy, &g_unk_7172a065d8, &g_unk_7101a19000);
    InitProperty(&g_unk_7172a065e0, g_unk_7101cb2828, g_unk_71726db788, "Enabled", refl_TypeOf<fn_710000d228>(),
                 fn_710111fa80, fn_71011173fc);
    InitProperty(&g_unk_7172a06608, g_unk_7101cb2828, g_unk_71726db788, "NumDeaths", refl_TypeOf<fn_7100014768>(),
                 fn_710111fa80, fn_71011173fc);
    fn_7101117478();
    fn_710009ec00(&g_unk_7172a06630, &tu.type);
    new (&g_unk_7172a06638) UnkValue(CStrId("vectSpawnPoints", true), refl_TypeOf<fn_7100009658>);
    fn_7100000250(fn_710010c424, &g_unk_7172a06638, &g_unk_7101a19000);
    new (&tu.metroid_key) CStrId(fn_7100de8f78(CStrId("SPAWNGROUPS:METROID", true)));
    fn_7100000250(CStrId_Destroy, &tu.metroid_key, &g_unk_7101a19000);
    new (&tu.key) CStrId(fn_7100de8f78(SpawnGroupId()));
    fn_7100000250(CStrId_Destroy, &tu.key, &g_unk_7101a19000);
}
