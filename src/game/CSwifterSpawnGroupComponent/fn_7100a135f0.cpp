// Copies of fn_710035d230 (src/game/SDeathImpactDisplacement/fn_710035d230.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_7100a135f0, fn_7100a17b30, fn_7100a4f6d0.
// Static initialiser of the unit of the ECutPathMode, EXpawnAngPolicy.
#include "../../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_710035d230 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_710035d230 g_unk_717274bb60 __attribute__((visibility("hidden")));
extern UnkStatics_710035d230 g_unk_7172744b00 __attribute__((visibility("hidden")));
extern UnkStatics_710035d230 g_unk_7172744158 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_717274b898;
extern UnkEnumType g_unk_7172744838;
extern UnkEnumType g_unk_7172743e90;
void fn_710035c5fc(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100a4e400();
void fn_7100a15414();
void fn_7100a11ab0();
extern char g_unk_717274b9b0[];
extern char g_unk_7172744950[];
extern char g_unk_7172743fa8[];
extern UnkConversion g_unk_717274b9b8;
extern UnkConversion g_unk_7172744958;
extern UnkConversion g_unk_7172743fb0;
extern UnkConversion g_unk_717274b9d8;
extern UnkConversion g_unk_7172744978;
extern UnkConversion g_unk_7172743fd0;
void fn_7100a4f4bc() __attribute__((visibility("hidden")));
void fn_7100a17928() __attribute__((visibility("hidden")));
void fn_7100a13384() __attribute__((visibility("hidden")));
void fn_7100a4f4c8() __attribute__((visibility("hidden")));
void fn_7100a17934() __attribute__((visibility("hidden")));
void fn_7100a13390() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_717274b9f8;
extern UnkEnumType g_unk_7172744998;
extern UnkEnumType g_unk_7172743ff0;
void fn_710035c9e8(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100a4e7ec();
void fn_7100a156e0();
void fn_7100a11d7c();
extern char g_unk_717274bb10[];
extern char g_unk_7172744ab0[];
extern char g_unk_7172744108[];
extern UnkConversion g_unk_717274bb18;
extern UnkConversion g_unk_7172744ab8;
extern UnkConversion g_unk_7172744110;
extern UnkConversion g_unk_717274bb38;
extern UnkConversion g_unk_7172744ad8;
extern UnkConversion g_unk_7172744130;
void fn_7100a4f4d4() __attribute__((visibility("hidden")));
void fn_7100a17940() __attribute__((visibility("hidden")));
void fn_7100a1339c() __attribute__((visibility("hidden")));
void fn_7100a4f4e0() __attribute__((visibility("hidden")));
void fn_7100a1794c() __attribute__((visibility("hidden")));
void fn_7100a133a8() __attribute__((visibility("hidden")));
extern char g_unk_717274bb58[];
extern char g_unk_7172744af8[];
extern char g_unk_7172744150[];

void fn_7100a11bfc(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100a11ec8(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100a15560(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100a1588c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100a4e66c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100a4e938(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100a135f0() {
    InitHeaderIds(&g_unk_7172744158.ids);
    InitEnum(&g_unk_7172743e90, "ESwifterSpawnGroupDirection", fn_7100a11bfc, fn_7100a11ab0,
             g_unk_7172743fa8, &g_unk_7172743fb0, fn_7100a13384, &g_unk_7172743fd0, fn_7100a13390);
    InitEnum(&g_unk_7172743ff0, "ESwifterSpawnGroupSpawnMode", fn_7100a11ec8, fn_7100a11d7c,
             g_unk_7172744108, &g_unk_7172744110, fn_7100a1339c, &g_unk_7172744130, fn_7100a133a8);
    fn_7100a1204c();
    fn_710009ec00(g_unk_7172744150, g_unk_7172744158.field_0x20);
}

void fn_7100a17b30() {
    InitHeaderIds(&g_unk_7172744b00.ids);
    InitEnum(&g_unk_7172744838, "ECaterzillaSpawnPointDir", fn_7100a15560, fn_7100a15414,
             g_unk_7172744950, &g_unk_7172744958, fn_7100a17928, &g_unk_7172744978, fn_7100a17934);
    InitEnum(&g_unk_7172744998, "ECaterzillaSpawnPointOrder", fn_7100a1588c, fn_7100a156e0,
             g_unk_7172744ab0, &g_unk_7172744ab8, fn_7100a17940, &g_unk_7172744ad8, fn_7100a1794c);
    fn_7100a15a10();
    fn_710009ec00(g_unk_7172744af8, g_unk_7172744b00.field_0x20);
}

void fn_7100a4f6d0() {
    InitHeaderIds(&g_unk_717274bb60.ids);
    InitEnum(&g_unk_717274b898, "navmesh::ENavMeshGroup", fn_7100a4e66c, fn_7100a4e400,
             g_unk_717274b9b0, &g_unk_717274b9b8, fn_7100a4f4bc, &g_unk_717274b9d8, fn_7100a4f4c8);
    InitEnum(&g_unk_717274b9f8, "CForbiddenEdgesLogicAction::EState", fn_7100a4e938, fn_7100a4e7ec,
             g_unk_717274bb10, &g_unk_717274bb18, fn_7100a4f4d4, &g_unk_717274bb38, fn_7100a4f4e0);
    fn_7100a4eabc();
    fn_710009ec00(g_unk_717274bb58, g_unk_717274bb60.field_0x20);
}

