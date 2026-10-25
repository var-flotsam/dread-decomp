// Static initialiser of the unit of the CCharClassTakumakuAIComponent::ESubspecies enum.
#include "UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71006d39c0 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    u8 field_0x28[8];
};
extern UnkStatics_71006d39c0 g_unk_71727099c8 __attribute__((visibility("hidden")));
extern char g_unk_7172709860[];
extern UnkEnumType g_unk_7172709868;
void fn_71006d3360(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71006d31b4();
extern char g_unk_7172709980[];
extern UnkConversion g_unk_7172709988;
extern UnkConversion g_unk_71727099a8;
void fn_71006d3864() __attribute__((visibility("hidden")));
void fn_71006d3870() __attribute__((visibility("hidden")));

void fn_71006d39c0() {
    InitHeaderIds(&g_unk_71727099c8.ids);
    fn_71006d2174();
    fn_710009ec00(g_unk_7172709860, g_unk_71727099c8.field_0x28);
    InitEnum(&g_unk_7172709868, "CCharClassTakumakuAIComponent::ESubspecies", fn_71006d3360, fn_71006d31b4,
             g_unk_7172709980, &g_unk_7172709988, fn_71006d3864, &g_unk_71727099a8, fn_71006d3870);
    InitId(&g_unk_71727099c8.field_0x20, "turn_relax");
}
