// Static initialiser of the unit of the ECutPathMode, EXpawnAngPolicy.
#include "../../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_710035d230 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_710035d230 g_unk_71726b2078 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726b1db0;
void fn_710035c5fc(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_710035c4b0();
extern char g_unk_71726b1ec8[];
extern UnkConversion g_unk_71726b1ed0;
extern UnkConversion g_unk_71726b1ef0;
void fn_710035d094() __attribute__((visibility("hidden")));
void fn_710035d0a0() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726b1f10;
void fn_710035c9e8(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_710035c77c();
extern char g_unk_71726b2028[];
extern UnkConversion g_unk_71726b2030;
extern UnkConversion g_unk_71726b2050;
void fn_710035d0ac() __attribute__((visibility("hidden")));
void fn_710035d0b8() __attribute__((visibility("hidden")));
extern char g_unk_71726b2070[];

void fn_710035d230() {
    InitHeaderIds(&g_unk_71726b2078.ids);
    InitEnum(&g_unk_71726b1db0, "ECutPathMode", fn_710035c5fc, fn_710035c4b0,
             g_unk_71726b1ec8, &g_unk_71726b1ed0, fn_710035d094, &g_unk_71726b1ef0, fn_710035d0a0);
    InitEnum(&g_unk_71726b1f10, "EXpawnAngPolicy", fn_710035c9e8, fn_710035c77c,
             g_unk_71726b2028, &g_unk_71726b2030, fn_710035d0ac, &g_unk_71726b2050, fn_710035d0b8);
    fn_710035cb6c();
    fn_710009ec00(g_unk_71726b2070, g_unk_71726b2078.field_0x20);
}
