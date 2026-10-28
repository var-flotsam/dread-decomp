// Copies of fn_71004f1820 (src/game/CEmmyCaveAIComponent/fn_71004f1820.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_7100a7f7b0, fn_71009101b0, fn_7100953fa0.
// Static initialiser of the unit of CEmmyCaveAIComponent: a string id, a property and the class
// registration.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71004f1820 {
    HeaderIds ids;
    u8 pad_0x18[8];
    u8 field_0x20[8];
};
extern UnkStatics_71004f1820 g_unk_7172750368 __attribute__((visibility("hidden")));
extern UnkStatics_71004f1820 g_unk_7172739fb0 __attribute__((visibility("hidden")));
extern UnkStatics_71004f1820 g_unk_7172735508 __attribute__((visibility("hidden")));
extern UnkStrIdBase g_unk_71727502f0;
extern UnkStrIdBase g_unk_7172739f38;
extern UnkStrIdBase g_unk_7172735480;
extern UnkProperty g_unk_71727502f8;
extern UnkProperty g_unk_7172739f40;
extern UnkProperty g_unk_7172735488;
extern void* g_unk_7101a91118[];
extern void* g_unk_7101a84c68[];
extern void* g_unk_7101a814e8[];
void fn_71004f1338(UnkProperty* p);
void fn_7100a7df30(void* p);
void fn_7100952bc0(void* p);
void fn_710090efb0(void* p);
extern char g_unk_7172750320[];
extern char g_unk_7172739f68[];
extern char g_unk_71727354b0[];

void fn_7100a7f500(UnkProperty* p);
void fn_710090ffb4(UnkProperty* p);
void fn_7100953d64(UnkProperty* p);
void fn_7100a7f7b0() {
    InitHeaderIds(&g_unk_7172750368.ids);
    InitId(&g_unk_71727502f0, "POWERGENERATOR");
    InitProperty(&g_unk_71727502f8, g_unk_7101a91118, g_unk_71726db788, "Used", TypeOf<fn_710000d228>(),
                 fn_7100a7f500, fn_7100a7df30);
    fn_7100a7dfac();
    fn_710009ec00(g_unk_7172750320, g_unk_7172750368.field_0x20);
}

void fn_71009101b0() {
    InitHeaderIds(&g_unk_7172735508.ids);
    InitId(&g_unk_7172735480, "LA_TRIGGER");
    InitProperty(&g_unk_7172735488, g_unk_7101a814e8, g_unk_71726db788, "Enabled", TypeOf<fn_710000d228>(),
                 fn_710090ffb4, fn_710090efb0);
    fn_710090f02c();
    fn_710009ec00(g_unk_71727354b0, g_unk_7172735508.field_0x20);
}

void fn_7100953fa0() {
    InitHeaderIds(&g_unk_7172739fb0.ids);
    InitId(&g_unk_7172739f38, "WAMP");
    InitProperty(&g_unk_7172739f40, g_unk_7101a84c68, g_unk_71726db788, "Finished", TypeOf<fn_710000d228>(),
                 fn_7100953d64, fn_7100952bc0);
    fn_7100952c3c();
    fn_710009ec00(g_unk_7172739f68, g_unk_7172739fb0.field_0x20);
}
