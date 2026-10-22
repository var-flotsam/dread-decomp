// Copies of fn_71002d4770 (src/game_s2/fn_71002d4770.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_7100a7dd80, fn_71009b6e00, fn_71009c68e0.
// Static initialiser of the unit of the animtree::CAnimatedNodeDef::EAnimNodeType.
#include "../game_s7/UnkStaticInitS7.h"

using namespace unk_static_init;

struct UnkStatics_71002d4770 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_71002d4770 g_unk_7172750198 __attribute__((visibility("hidden")));
extern UnkStatics_71002d4770 g_unk_717273f0b8 __attribute__((visibility("hidden")));
extern UnkStatics_71002d4770 g_unk_717273d180 __attribute__((visibility("hidden")));
void* fn_71002d32f8();  // animtree::CAnimatedNodeDef::refl_GetClass
extern char g_unk_7172750030[];
extern char g_unk_717273ef50[];
extern char g_unk_717273d018[];
extern UnkEnumType g_unk_7172750038;
extern UnkEnumType g_unk_717273ef58;
extern UnkEnumType g_unk_717273d020;
void fn_71002d3b70(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100a7c484();
void fn_71009c5668();
void fn_71009b5aec();
extern char g_unk_7172750150[];
extern char g_unk_717273f070[];
extern char g_unk_717273d138[];
extern UnkConversion g_unk_7172750158;
extern UnkConversion g_unk_717273f078;
extern UnkConversion g_unk_717273d140;
extern UnkConversion g_unk_7172750178;
extern UnkConversion g_unk_717273f098;
extern UnkConversion g_unk_717273d160;
void fn_7100a7dbe4() __attribute__((visibility("hidden")));
void fn_71009c6714() __attribute__((visibility("hidden")));
void fn_71009b6cf0() __attribute__((visibility("hidden")));
void fn_7100a7dbf0() __attribute__((visibility("hidden")));
void fn_71009c6720() __attribute__((visibility("hidden")));
void fn_71009b6cfc() __attribute__((visibility("hidden")));

void* fn_7100a7bfc4();
void fn_7100a7c630(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void* fn_71009b57c4();
void fn_71009b5c38(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void* fn_71009c53f0();
void fn_71009c5814(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100a7dd80() {
    InitHeaderIds(&g_unk_7172750198.ids);
    fn_7100a7bfc4();
    fn_710009ec00(g_unk_7172750030, g_unk_7172750198.field_0x20);
    InitEnum(&g_unk_7172750038, "CElectricGeneratorComponent::EBlackOutZone", fn_7100a7c630, fn_7100a7c484,
             g_unk_7172750150, &g_unk_7172750158, fn_7100a7dbe4, &g_unk_7172750178, fn_7100a7dbf0);
}

void fn_71009b6e00() {
    InitHeaderIds(&g_unk_717273d180.ids);
    fn_71009b57c4();
    fn_710009ec00(g_unk_717273d018, g_unk_717273d180.field_0x20);
    InitEnum(&g_unk_717273d020, "CSideEnemyMovement::EDir", fn_71009b5c38, fn_71009b5aec,
             g_unk_717273d138, &g_unk_717273d140, fn_71009b6cf0, &g_unk_717273d160, fn_71009b6cfc);
}

void fn_71009c68e0() {
    InitHeaderIds(&g_unk_717273f0b8.ids);
    fn_71009c53f0();
    fn_710009ec00(g_unk_717273ef50, g_unk_717273f0b8.field_0x20);
    InitEnum(&g_unk_717273ef58, "CNavMeshItemStage::ESide", fn_71009c5814, fn_71009c5668,
             g_unk_717273f070, &g_unk_717273f078, fn_71009c6714, &g_unk_717273f098, fn_71009c6720);
}

