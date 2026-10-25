// Static initialiser of the unit of the CCharClassDropterAIComponent::ESubspecies.
#include "../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71004ebd30 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_71004ebd30 g_unk_71726daf80 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726dae18;
void fn_71004eb14c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71004eafa0();
extern char g_unk_71726daf30[];
extern UnkConversion g_unk_71726daf38;
extern UnkConversion g_unk_71726daf58;
void fn_71004ebc1c() __attribute__((visibility("hidden")));
void fn_71004ebc28() __attribute__((visibility("hidden")));
extern char g_unk_71726daf78[];

void fn_71004ebd30() {
    InitHeaderIds(&g_unk_71726daf80.ids);
    InitEnum(&g_unk_71726dae18, "CCharClassDropterAIComponent::ESubspecies", fn_71004eb14c, fn_71004eafa0,
             g_unk_71726daf30, &g_unk_71726daf38, fn_71004ebc1c, &g_unk_71726daf58, fn_71004ebc28);
    fn_71004eb2d0();
    fn_710009ec00(g_unk_71726daf78, g_unk_71726daf80.field_0x20);
}
