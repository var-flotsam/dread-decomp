// Static initialiser of the unit of the CChozoZombieXPoisonClawsAttack::EState.
#include "../../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_710049f440 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    u8 field_0x30[8];
};
extern UnkStatics_710049f440 g_unk_71726d16e8 __attribute__((visibility("hidden")));
extern char g_unk_71726d1580[];
extern UnkEnumType g_unk_71726d1588;
void fn_710049e34c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_710049e0e0();
extern char g_unk_71726d16a0[];
extern UnkConversion g_unk_71726d16a8;
extern UnkConversion g_unk_71726d16c8;
void fn_710049ee80() __attribute__((visibility("hidden")));
void fn_710049ee8c() __attribute__((visibility("hidden")));

void fn_710049f440() {
    InitHeaderIds(&g_unk_71726d16e8.ids);
    fn_710049de64();
    fn_710009ec00(g_unk_71726d1580, g_unk_71726d16e8.field_0x30);
    InitEnum(&g_unk_71726d1588, "CChozoZombieXPoisonClawsAttack::EState", fn_710049e34c, fn_710049e0e0,
             g_unk_71726d16a0, &g_unk_71726d16a8, fn_710049ee80, &g_unk_71726d16c8, fn_710049ee8c);
    InitId(&g_unk_71726d16e8.field_0x20, "attack_1_init");
    InitId(&g_unk_71726d16e8.field_0x28, "fDamageChozoZombieXPoisonClaws");
}
