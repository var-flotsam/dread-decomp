// Static initialiser of the unit of the CAutoolAIComponent::EThrusterMode enum.
#include "../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71003c7230 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    u8 pad_0x30[8];
    u8 field_0x38[8];
};
extern UnkStatics_71003c7230 g_unk_71726be9a0 __attribute__((visibility("hidden")));
extern char g_unk_71726be828[];
extern UnkEnumType g_unk_71726be830;
void fn_71003c2f9c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71003c2df0();
extern char g_unk_71726be948[];
extern UnkConversion g_unk_71726be950;
extern UnkConversion g_unk_71726be970;
void fn_71003c6db8() __attribute__((visibility("hidden")));
void fn_71003c6dc4() __attribute__((visibility("hidden")));

void fn_71003c7230() {
    InitHeaderIds(&g_unk_71726be9a0.ids);
    InitId(&g_unk_71726be9a0.field_0x18, "right_thruster");
    InitId(&g_unk_71726be9a0.field_0x20, "left_thruster");
    InitId(&g_unk_71726be9a0.field_0x28, "bottom_thruster");
    fn_71003c2a94();
    fn_710009ec00(g_unk_71726be828, g_unk_71726be9a0.field_0x38);
    InitEnum(&g_unk_71726be830, "CAutoolAIComponent::EThrusterMode", fn_71003c2f9c, fn_71003c2df0,
             g_unk_71726be948, &g_unk_71726be950, fn_71003c6db8, &g_unk_71726be970, fn_71003c6dc4);
}
