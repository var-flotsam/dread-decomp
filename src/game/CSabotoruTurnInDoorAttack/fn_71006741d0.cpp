// Static initialiser of the unit of the CSabotoruTurnInDoorAttack::EState enum.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71006741d0 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    u8 field_0x28[8];
};
extern UnkStatics_71006741d0 g_unk_71726fffe0 __attribute__((visibility("hidden")));
extern char g_unk_71726ffe78[];
extern UnkEnumType g_unk_71726ffe80;
void fn_710067307c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100672e10();
extern char g_unk_71726fff98[];
extern UnkConversion g_unk_71726fffa0;
extern UnkConversion g_unk_71726fffc0;
void fn_7100673d40() __attribute__((visibility("hidden")));
void fn_7100673d4c() __attribute__((visibility("hidden")));

void fn_71006741d0() {
    InitHeaderIds(&g_unk_71726fffe0.ids);
    fn_7100672b94();
    fn_710009ec00(g_unk_71726ffe78, g_unk_71726fffe0.field_0x28);
    InitEnum(&g_unk_71726ffe80, "CSabotoruTurnInDoorAttack::EState", fn_710067307c, fn_7100672e10,
             g_unk_71726fff98, &g_unk_71726fffa0, fn_7100673d40, &g_unk_71726fffc0, fn_7100673d4c);
    InitId(&g_unk_71726fffe0.field_0x20, "initattack");
}
