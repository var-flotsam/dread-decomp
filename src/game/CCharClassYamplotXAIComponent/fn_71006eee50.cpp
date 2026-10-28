// Static initialiser of the unit of the CCharClassYamplotXAIComponent::ESubspecies enum.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71006eee50 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    u8 field_0x30[8];
};
extern UnkStatics_71006eee50 g_unk_717270c300 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_717270c198;
void fn_71006ee52c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71006ee3e0();
extern char g_unk_717270c2b0[];
extern UnkConversion g_unk_717270c2b8;
extern UnkConversion g_unk_717270c2d8;
void fn_71006eed30() __attribute__((visibility("hidden")));
void fn_71006eed3c() __attribute__((visibility("hidden")));
extern char g_unk_717270c2f8[];

void fn_71006eee50() {
    InitHeaderIds(&g_unk_717270c300.ids);
    InitEnum(&g_unk_717270c198, "CCharClassYamplotXAIComponent::ESubspecies", fn_71006ee52c, fn_71006ee3e0,
             g_unk_717270c2b0, &g_unk_717270c2b8, fn_71006eed30, &g_unk_717270c2d8, fn_71006eed3c);
    fn_71006ee6b0();
    fn_710009ec00(g_unk_717270c2f8, g_unk_717270c300.field_0x30);
    InitId(&g_unk_717270c300.field_0x20, "walk_turn180");
    InitId(&g_unk_717270c300.field_0x28, "push_onair");
}
