// Static initialiser of the unit of the CCharClassQuetzoaAIComponent::ESubspecies enum.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUProperty.h"

using namespace unk_static_init;

struct UnkStatics_7100649430 {
    HeaderIds ids;
    u8 pad_0x18[0x10];
    u8 field_0x28[8];
    u8 pad_0x30[0x130];
    u8 field_0x160[8];
};
extern UnkStatics_7100649430 g_unk_71726fbd88 __attribute__((visibility("hidden")));
extern char g_unk_71726fbbe8[];
extern UnkTunable g_unk_71726fbbf0;
extern void* g_unk_7101a5a210[];
void fn_7100075104(void* self);  // its destructor, as registered
extern UnkEnumType g_unk_71726fbc20;
void fn_7100648b5c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100648a10();
extern char g_unk_71726fbd38[];
extern UnkConversion g_unk_71726fbd40;
extern UnkConversion g_unk_71726fbd60;
void fn_7100649274() __attribute__((visibility("hidden")));
void fn_7100649280() __attribute__((visibility("hidden")));
extern char g_unk_71726fbd80[];

void fn_7100649430() {
    InitHeaderIds(&g_unk_71726fbd88.ids);
    fn_71006483b4();
    fn_710009ec00(g_unk_71726fbbe8, g_unk_71726fbd88.field_0x28);
    InitTunable(&g_unk_71726fbbf0, fn_7100136ecc, g_unk_7101a5a210, fn_7100075104);
    InitEnum(&g_unk_71726fbc20, "CCharClassQuetzoaAIComponent::ESubspecies", fn_7100648b5c, fn_7100648a10,
             g_unk_71726fbd38, &g_unk_71726fbd40, fn_7100649274, &g_unk_71726fbd60, fn_7100649280);
    fn_7100648ce0();
    fn_710009ec00(g_unk_71726fbd80, g_unk_71726fbd88.field_0x160);
}
