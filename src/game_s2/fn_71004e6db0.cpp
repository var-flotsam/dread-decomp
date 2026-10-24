// Static initialiser of the unit of the CCharClassDredhedAIComponent::ESubspecies.
#include "../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71004e6db0 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_71004e6db0 g_unk_71726da5e0 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726da478;
void fn_71004e65ec(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71004e64a0();
extern char g_unk_71726da590[];
extern UnkConversion g_unk_71726da598;
extern UnkConversion g_unk_71726da5b8;
void fn_71004e6ca8() __attribute__((visibility("hidden")));
void fn_71004e6cb4() __attribute__((visibility("hidden")));
extern char g_unk_71726da5d8[];

void fn_71004e6db0() {
    InitHeaderIds(&g_unk_71726da5e0.ids);
    InitEnum(&g_unk_71726da478, "CCharClassDredhedAIComponent::ESubspecies", fn_71004e65ec, fn_71004e64a0,
             g_unk_71726da590, &g_unk_71726da598, fn_71004e6ca8, &g_unk_71726da5b8, fn_71004e6cb4);
    fn_71004e6770();
    fn_710009ec00(g_unk_71726da5d8, g_unk_71726da5e0.field_0x20);
}
