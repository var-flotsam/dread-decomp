// Static initialiser of the unit of the CBigFistAttack::EBigFistAttackState enum.
#include "../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71003e1840 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    UnkStrIdBase field_0x38;
    UnkStrIdBase field_0x40;
    UnkStrIdBase field_0x48;
    u8 field_0x50[8];
};
extern UnkStatics_71003e1840 g_unk_71726c0518 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726c03b0;
void fn_71003e045c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71003e0250();
extern char g_unk_71726c04c8[];
extern UnkConversion g_unk_71726c04d0;
extern UnkConversion g_unk_71726c04f0;
void fn_71003e13b8() __attribute__((visibility("hidden")));
void fn_71003e13c4() __attribute__((visibility("hidden")));
extern char g_unk_71726c0510[];

void fn_71003e1840() {
    InitHeaderIds(&g_unk_71726c0518.ids);
    InitEnum(&g_unk_71726c03b0, "CBigFistAttack::EBigFistAttackState", fn_71003e045c, fn_71003e0250,
             g_unk_71726c04c8, &g_unk_71726c04d0, fn_71003e13b8, &g_unk_71726c04f0, fn_71003e13c4);
    fn_71003e05e0();
    fn_710009ec00(g_unk_71726c0510, g_unk_71726c0518.field_0x50);
    InitId(&g_unk_71726c0518.field_0x20, "attack_init4m");
    InitId(&g_unk_71726c0518.field_0x28, "attack_init5m");
    InitId(&g_unk_71726c0518.field_0x30, "attack_init6m");
    InitId(&g_unk_71726c0518.field_0x38, "attack4m");
    InitId(&g_unk_71726c0518.field_0x40, "attack5m");
    InitId(&g_unk_71726c0518.field_0x48, "attack6m");
}
