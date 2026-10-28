// Static initialiser of the unit of the CCharClassHecathonAIComponent::ESubspecies enum.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUProperty.h"

using namespace unk_static_init;

struct UnkStatics_71005a7870 {
    HeaderIds ids;
    u8 pad_0x18[0x10];
    u8 field_0x28[8];
    u8 pad_0x30[0x130];
    u8 field_0x160[8];
};
extern UnkStatics_71005a7870 g_unk_71726e8d70 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726e8b98;
void fn_71005a610c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71005a5fc0();
extern char g_unk_71726e8cb0[];
extern UnkConversion g_unk_71726e8cb8;
extern UnkConversion g_unk_71726e8cd8;
void fn_71005a7694() __attribute__((visibility("hidden")));
void fn_71005a76a0() __attribute__((visibility("hidden")));
extern char g_unk_71726e8cf8[];
extern char g_unk_71726e8cf9[];
extern UnkTunable g_unk_71726e8d00;
extern void* g_unk_7101a4f768[];
void fn_7100075104(void* self);  // its destructor, as registered

void fn_71005a7870() {
    InitHeaderIds(&g_unk_71726e8d70.ids);
    InitEnum(&g_unk_71726e8b98, "CCharClassHecathonAIComponent::ESubspecies", fn_71005a610c, fn_71005a5fc0,
             g_unk_71726e8cb0, &g_unk_71726e8cb8, fn_71005a7694, &g_unk_71726e8cd8, fn_71005a76a0);
    fn_71005a6290();
    fn_710009ec00(g_unk_71726e8cf8, g_unk_71726e8d70.field_0x28);
    fn_71005a65bc();
    fn_710009ec00(g_unk_71726e8cf9, g_unk_71726e8d70.field_0x160);
    InitTunable(&g_unk_71726e8d00, fn_7100136ecc, g_unk_7101a4f768, fn_7100075104);
}
