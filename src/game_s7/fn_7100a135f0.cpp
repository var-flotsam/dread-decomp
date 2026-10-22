// Copies of fn_710035d230 (src/game_s2/fn_710035d230.cpp) for other types, with their own callees and data (the names, strings and comments below are the original's): fn_7100a135f0, fn_7100a17b30, fn_7100a4f6d0.
// Static initialiser of the unit of the ECutPathMode, EXpawnAngPolicy.
#include "../game_s7/UnkStaticInitS7.h"

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
void* fn_710035cb6c();  // SDeathImpactDisplacement::refl_GetClass
extern char g_unk_71726b2070[];

void fn_7100a11bfc(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100a11ec8(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void* fn_7100a1204c();
void fn_7100a15560(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100a1588c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void* fn_7100a15a10();
void fn_7100a4e66c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100a4e938(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void* fn_7100a4eabc();
void fn_7100a135f0() {
    InitHeaderIds(&g_unk_71726b2078.ids);
    InitEnum(&g_unk_71726b1db0, "ECutPathMode", fn_7100a11bfc, fn_710035c4b0,
             g_unk_71726b1ec8, &g_unk_71726b1ed0, fn_710035d094, &g_unk_71726b1ef0, fn_710035d0a0);
    InitEnum(&g_unk_71726b1f10, "EXpawnAngPolicy", fn_7100a11ec8, fn_710035c77c,
             g_unk_71726b2028, &g_unk_71726b2030, fn_710035d0ac, &g_unk_71726b2050, fn_710035d0b8);
    fn_7100a1204c();
    fn_710009ec00(g_unk_71726b2070, g_unk_71726b2078.field_0x20);
}

void fn_7100a17b30() {
    InitHeaderIds(&g_unk_71726b2078.ids);
    InitEnum(&g_unk_71726b1db0, "ECutPathMode", fn_7100a15560, fn_710035c4b0,
             g_unk_71726b1ec8, &g_unk_71726b1ed0, fn_710035d094, &g_unk_71726b1ef0, fn_710035d0a0);
    InitEnum(&g_unk_71726b1f10, "EXpawnAngPolicy", fn_7100a1588c, fn_710035c77c,
             g_unk_71726b2028, &g_unk_71726b2030, fn_710035d0ac, &g_unk_71726b2050, fn_710035d0b8);
    fn_7100a15a10();
    fn_710009ec00(g_unk_71726b2070, g_unk_71726b2078.field_0x20);
}

void fn_7100a4f6d0() {
    InitHeaderIds(&g_unk_71726b2078.ids);
    InitEnum(&g_unk_71726b1db0, "ECutPathMode", fn_7100a4e66c, fn_710035c4b0,
             g_unk_71726b1ec8, &g_unk_71726b1ed0, fn_710035d094, &g_unk_71726b1ef0, fn_710035d0a0);
    InitEnum(&g_unk_71726b1f10, "EXpawnAngPolicy", fn_7100a4e938, fn_710035c77c,
             g_unk_71726b2028, &g_unk_71726b2030, fn_710035d0ac, &g_unk_71726b2050, fn_710035d0b8);
    fn_7100a4eabc();
    fn_710009ec00(g_unk_71726b2070, g_unk_71726b2078.field_0x20);
}

