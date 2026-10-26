// Static initialiser of the unit of the CCharClassGooplotAIComponent::ESubspecies enum.
#include "UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUProperty.h"

using namespace unk_static_init;

struct UnkStatics_71005994a0 {
    HeaderIds ids;
    u8 pad_0x18[0x10];
    u8 field_0x28[8];
    u8 pad_0x30[0x130];
    u8 field_0x160[8];
};
extern UnkStatics_71005994a0 g_unk_71726e7480 __attribute__((visibility("hidden")));
extern char g_unk_71726e72b0[];
extern UnkTunable g_unk_71726e72b8;
extern void* g_unk_7101a4d090[];
void fn_7100075104(void* self);  // its destructor, as registered
extern UnkEnumType g_unk_71726e7318;
void fn_7100598b1c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71005989d0();
extern char g_unk_71726e7430[];
extern UnkConversion g_unk_71726e7438;
extern UnkConversion g_unk_71726e7458;
void fn_71005992e0() __attribute__((visibility("hidden")));
void fn_71005992ec() __attribute__((visibility("hidden")));
extern char g_unk_71726e7478[];

void fn_71005994a0() {
    InitHeaderIds(&g_unk_71726e7480.ids);
    fn_71005972bc();
    fn_710009ec00(g_unk_71726e72b0, g_unk_71726e7480.field_0x28);
    InitTunable(&g_unk_71726e72b8, fn_7100136ecc, g_unk_7101a4d090, fn_7100075104);
    InitEnum(&g_unk_71726e7318, "CCharClassGooplotAIComponent::ESubspecies", fn_7100598b1c, fn_71005989d0,
             g_unk_71726e7430, &g_unk_71726e7438, fn_71005992e0, &g_unk_71726e7458, fn_71005992ec);
    fn_7100598ca0();
    fn_710009ec00(g_unk_71726e7478, g_unk_71726e7480.field_0x160);
}
