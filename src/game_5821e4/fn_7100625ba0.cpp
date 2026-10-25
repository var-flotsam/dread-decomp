// Static initialiser of the unit of the CNailuggerAcidBallsAttack::EState enum.
#include "UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_7100625ba0 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    u8 field_0x28[8];
};
extern UnkStatics_7100625ba0 g_unk_71726f8550 __attribute__((visibility("hidden")));
extern char g_unk_71726f83e8[];
extern UnkEnumType g_unk_71726f83f0;
void fn_71006247ac(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71006245a0();
extern char g_unk_71726f8508[];
extern UnkConversion g_unk_71726f8510;
extern UnkConversion g_unk_71726f8530;
void fn_7100625710() __attribute__((visibility("hidden")));
void fn_710062571c() __attribute__((visibility("hidden")));

void fn_7100625ba0() {
    InitHeaderIds(&g_unk_71726f8550.ids);
    fn_7100624324();
    fn_710009ec00(g_unk_71726f83e8, g_unk_71726f8550.field_0x28);
    InitEnum(&g_unk_71726f83f0, "CNailuggerAcidBallsAttack::EState", fn_71006247ac, fn_71006245a0,
             g_unk_71726f8508, &g_unk_71726f8510, fn_7100625710, &g_unk_71726f8530, fn_710062571c);
    InitId(&g_unk_71726f8550.field_0x20, "nailuggeracidball");
}
