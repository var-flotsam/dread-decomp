// Static initialiser of the unit of the CBigkranXMagmaRainAttack::EState.
#include "../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71003eb3f0 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    u8 field_0x28[8];
};
extern UnkStatics_71003eb3f0 g_unk_71726c1800 __attribute__((visibility("hidden")));
extern char g_unk_71726c1698[];
extern UnkEnumType g_unk_71726c16a0;
void fn_71003ea384(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71003ea178();
extern char g_unk_71726c17b8[];
extern UnkConversion g_unk_71726c17c0;
extern UnkConversion g_unk_71726c17e0;
void fn_71003eaf60() __attribute__((visibility("hidden")));
void fn_71003eaf6c() __attribute__((visibility("hidden")));

void fn_71003eb3f0() {
    InitHeaderIds(&g_unk_71726c1800.ids);
    fn_71003e9efc();
    fn_710009ec00(g_unk_71726c1698, g_unk_71726c1800.field_0x28);
    InitEnum(&g_unk_71726c16a0, "CBigkranXMagmaRainAttack::EState", fn_71003ea384, fn_71003ea178,
             g_unk_71726c17b8, &g_unk_71726c17c0, fn_71003eaf60, &g_unk_71726c17e0, fn_71003eaf6c);
    InitId(&g_unk_71726c1800.field_0x20, "attack_magmarain_attack-init");
}
