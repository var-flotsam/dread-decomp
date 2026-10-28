// Static initialiser of the unit of the animtree::CAnimatedNodeDef::EAnimNodeType.
#include "../../../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71002d4770 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_71002d4770 g_unk_71726a9100 __attribute__((visibility("hidden")));
extern char g_unk_71726a8f98[];
extern UnkEnumType g_unk_71726a8fa0;
void fn_71002d3b70(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71002d3a24();
extern char g_unk_71726a90b8[];
extern UnkConversion g_unk_71726a90c0;
extern UnkConversion g_unk_71726a90e0;
void fn_71002d3ef4() __attribute__((visibility("hidden")));
void fn_71002d3f00() __attribute__((visibility("hidden")));

void fn_71002d4770() {
    InitHeaderIds(&g_unk_71726a9100.ids);
    fn_71002d32f8();
    fn_710009ec00(g_unk_71726a8f98, g_unk_71726a9100.field_0x20);
    InitEnum(&g_unk_71726a8fa0, "animtree::CAnimatedNodeDef::EAnimNodeType", fn_71002d3b70, fn_71002d3a24,
             g_unk_71726a90b8, &g_unk_71726a90c0, fn_71002d3ef4, &g_unk_71726a90e0, fn_71002d3f00);
}
