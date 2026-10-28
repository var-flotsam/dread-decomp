// Static initialiser of the unit of the CNailongAIComponent::EType enum.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71006225e0 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    u8 pad_0x20[0x8];
    u8 field_0x28[8];
};
extern UnkStatics_71006225e0 g_unk_71726f7e40 __attribute__((visibility("hidden")));
extern char g_unk_71726f7cd8[];
extern UnkEnumType g_unk_71726f7ce0;
void fn_710061f9e8(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_710061f83c();
extern char g_unk_71726f7df8[];
extern UnkConversion g_unk_71726f7e00;
extern UnkConversion g_unk_71726f7e20;
void fn_710062228c() __attribute__((visibility("hidden")));
void fn_7100622298() __attribute__((visibility("hidden")));

void fn_71006225e0() {
    InitHeaderIds(&g_unk_71726f7e40.ids);
    InitId(&g_unk_71726f7e40.field_0x18, "CNailongAIComponent Custom wings layer freeze");
    fn_710061f464();
    fn_710009ec00(g_unk_71726f7cd8, g_unk_71726f7e40.field_0x28);
    InitEnum(&g_unk_71726f7ce0, "CNailongAIComponent::EType", fn_710061f9e8, fn_710061f83c,
             g_unk_71726f7df8, &g_unk_71726f7e00, fn_710062228c, &g_unk_71726f7e20, fn_7100622298);
}
