// Copies of fn_71004f1820 (src/game_s2/fn_71004f1820.cpp) for other types, with their own callees and data (the names, strings and comments below are the original's): fn_7100a7f7b0, fn_71009101b0, fn_7100953fa0.
// Static initialiser of the unit of CEmmyCaveAIComponent: a string id, a property and the class
// registration.
#include "../game_5821e4/UnkStaticInit.h"

using namespace unk_static_init;

struct UnkStatics_71004f1820 {
    HeaderIds ids;
    u8 pad_0x18[8];
    u8 field_0x20[8];
};
extern UnkStatics_71004f1820 g_unk_71726db7c0 __attribute__((visibility("hidden")));
extern UnkStrIdBase g_unk_71726db740;
void* fn_710000d228();  // the bool type's getter
extern UnkProperty g_unk_71726db748;
extern UnkProperty* g_unk_71726db788;
extern void* g_unk_7101a438b8[];
void fn_71004f1338(UnkProperty* p);
void fn_71004f0038(void* p);
void* fn_71004f00b4();  // CEmmyCaveAIComponent::refl_GetClass
extern char g_unk_71726db770[];

void fn_7100a7f500(UnkProperty* p);
void* fn_7100a7dfac();
void fn_710090ffb4(UnkProperty* p);
void* fn_710090f02c();
void fn_7100953d64(UnkProperty* p);
void* fn_7100952c3c();
void fn_7100a7f7b0() {
    InitHeaderIds(&g_unk_71726db7c0.ids);
    InitId(&g_unk_71726db740, "EMMY_CAVE");
    InitProperty(&g_unk_71726db748, g_unk_7101a438b8, g_unk_71726db788, "ForceGrab2", fn_710000d228(),
                 fn_7100a7f500, fn_71004f0038);
    fn_7100a7dfac();
    fn_710009ec00(g_unk_71726db770, g_unk_71726db7c0.field_0x20);
}

void fn_71009101b0() {
    InitHeaderIds(&g_unk_71726db7c0.ids);
    InitId(&g_unk_71726db740, "EMMY_CAVE");
    InitProperty(&g_unk_71726db748, g_unk_7101a438b8, g_unk_71726db788, "ForceGrab2", fn_710000d228(),
                 fn_710090ffb4, fn_71004f0038);
    fn_710090f02c();
    fn_710009ec00(g_unk_71726db770, g_unk_71726db7c0.field_0x20);
}

void fn_7100953fa0() {
    InitHeaderIds(&g_unk_71726db7c0.ids);
    InitId(&g_unk_71726db740, "EMMY_CAVE");
    InitProperty(&g_unk_71726db748, g_unk_7101a438b8, g_unk_71726db788, "ForceGrab2", fn_710000d228(),
                 fn_7100953d64, fn_71004f0038);
    fn_7100952c3c();
    fn_710009ec00(g_unk_71726db770, g_unk_71726db7c0.field_0x20);
}

