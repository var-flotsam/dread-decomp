// Static initialiser of the unit of the CGrappleBeamGun::ESelectionMode, CGrappleBeamGun::ESelectionInput enums.
#include "UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_7100834340 {
    HeaderIds ids;
    u8 pad_0x18[0x10];
    u8 field_0x28[8];
    u8 pad_0x30[0x130];
    u8 field_0x160[8];
};
extern UnkStatics_7100834340 g_unk_7172725c08 __attribute__((visibility("hidden")));
extern char g_unk_71727258f8[];
extern UnkEnumType g_unk_7172725900;
void fn_710083251c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71008322b0();
extern char g_unk_7172725a18[];
extern UnkConversion g_unk_7172725a20;
extern UnkConversion g_unk_7172725a40;
void fn_71008340b0() __attribute__((visibility("hidden")));
void fn_71008340bc() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_7172725a60;
void fn_7100832848(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_710083269c();
extern char g_unk_7172725b78[];
extern UnkConversion g_unk_7172725b80;
extern UnkConversion g_unk_7172725ba0;
void fn_71008340c8() __attribute__((visibility("hidden")));
void fn_71008340d4() __attribute__((visibility("hidden")));
extern char g_unk_7172725bc0[];
extern UnkTunable g_unk_7172725bc8;
void fn_7100136ecc(UnkTunable* self);  // the base constructor
extern void* g_unk_7101a77950[];
void fn_7100075104(void* self);  // its destructor, as registered

void fn_7100834340() {
    InitHeaderIds(&g_unk_7172725c08.ids);
    fn_7100832034();
    fn_710009ec00(g_unk_71727258f8, g_unk_7172725c08.field_0x28);
    InitEnum(&g_unk_7172725900, "CGrappleBeamGun::ESelectionMode", fn_710083251c, fn_71008322b0,
             g_unk_7172725a18, &g_unk_7172725a20, fn_71008340b0, &g_unk_7172725a40, fn_71008340bc);
    InitEnum(&g_unk_7172725a60, "CGrappleBeamGun::ESelectionInput", fn_7100832848, fn_710083269c,
             g_unk_7172725b78, &g_unk_7172725b80, fn_71008340c8, &g_unk_7172725ba0, fn_71008340d4);
    fn_71008329cc();
    fn_710009ec00(g_unk_7172725bc0, g_unk_7172725c08.field_0x160);
    InitTunable(&g_unk_7172725bc8, fn_7100136ecc, g_unk_7101a77950, fn_7100075104);
}
