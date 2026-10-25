// Static initialiser of the unit of the CCentralUnitCannonAIComponent::EState.
#include "../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71003fe930 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    u8 pad_0x30[0x50];
    u8 field_0x80[8];
};
extern UnkStatics_71003fe930 g_unk_71726c30e0 __attribute__((visibility("hidden")));
extern char g_unk_71726c2f78[];
extern UnkEnumType g_unk_71726c2f80;
void fn_71003fb528(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71003fb1fc();
extern char g_unk_71726c3098[];
extern UnkConversion g_unk_71726c30a0;
extern UnkConversion g_unk_71726c30c0;
void fn_71003fe278() __attribute__((visibility("hidden")));
void fn_71003fe284() __attribute__((visibility("hidden")));

void fn_71003fe930() {
    InitHeaderIds(&g_unk_71726c30e0.ids);
    fn_71003fafa0();
    fn_710009ec00(g_unk_71726c2f78, g_unk_71726c30e0.field_0x80);
    InitEnum(&g_unk_71726c2f80, "CCentralUnitCannonAIComponent::EState", fn_71003fb528, fn_71003fb1fc,
             g_unk_71726c3098, &g_unk_71726c30a0, fn_71003fe278, &g_unk_71726c30c0, fn_71003fe284);
    InitId(&g_unk_71726c30e0.field_0x20, "cannon");
    InitId(&g_unk_71726c30e0.field_0x28, "cucannonbeam");
}
