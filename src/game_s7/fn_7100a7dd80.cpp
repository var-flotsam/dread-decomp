// Copies of fn_71002d4770 (src/game_s2/fn_71002d4770.cpp) for other types, with their own callees and data (the names, strings and comments below are the original's): fn_7100a7dd80, fn_71009b6e00, fn_71009c68e0.
// Static initialiser of the unit of the animtree::CAnimatedNodeDef::EAnimNodeType.
#include "../game_s7/UnkStaticInitS7.h"

using namespace unk_static_init;

struct UnkStatics_71002d4770 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_71002d4770 g_unk_71726a9100 __attribute__((visibility("hidden")));
void* fn_71002d32f8();  // animtree::CAnimatedNodeDef::refl_GetClass
extern char g_unk_71726a8f98[];
extern UnkEnumType g_unk_71726a8fa0;
void fn_71002d3b70(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71002d3a24();
extern char g_unk_71726a90b8[];
extern UnkConversion g_unk_71726a90c0;
extern UnkConversion g_unk_71726a90e0;
void fn_71002d3ef4() __attribute__((visibility("hidden")));
void fn_71002d3f00() __attribute__((visibility("hidden")));

void* fn_7100a7bfc4();
void fn_7100a7c630(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void* fn_71009b57c4();
void fn_71009b5c38(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void* fn_71009c53f0();
void fn_71009c5814(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100a7dd80() {
    InitHeaderIds(&g_unk_71726a9100.ids);
    fn_7100a7bfc4();
    fn_710009ec00(g_unk_71726a8f98, g_unk_71726a9100.field_0x20);
    InitEnum(&g_unk_71726a8fa0, "animtree::CAnimatedNodeDef::EAnimNodeType", fn_7100a7c630, fn_71002d3a24,
             g_unk_71726a90b8, &g_unk_71726a90c0, fn_71002d3ef4, &g_unk_71726a90e0, fn_71002d3f00);
}

void fn_71009b6e00() {
    InitHeaderIds(&g_unk_71726a9100.ids);
    fn_71009b57c4();
    fn_710009ec00(g_unk_71726a8f98, g_unk_71726a9100.field_0x20);
    InitEnum(&g_unk_71726a8fa0, "animtree::CAnimatedNodeDef::EAnimNodeType", fn_71009b5c38, fn_71002d3a24,
             g_unk_71726a90b8, &g_unk_71726a90c0, fn_71002d3ef4, &g_unk_71726a90e0, fn_71002d3f00);
}

void fn_71009c68e0() {
    InitHeaderIds(&g_unk_71726a9100.ids);
    fn_71009c53f0();
    fn_710009ec00(g_unk_71726a8f98, g_unk_71726a9100.field_0x20);
    InitEnum(&g_unk_71726a8fa0, "animtree::CAnimatedNodeDef::EAnimNodeType", fn_71009c5814, fn_71002d3a24,
             g_unk_71726a90b8, &g_unk_71726a90c0, fn_71002d3ef4, &g_unk_71726a90e0, fn_71002d3f00);
}

