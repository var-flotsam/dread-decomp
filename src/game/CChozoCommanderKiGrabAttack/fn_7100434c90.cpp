// Static initialiser of the unit of the CChozoCommanderKiGrabAttack::SState.
#include "../../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_7100434c90 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    u8 pad_0x28[0x18];
    u8 field_0x40[8];
};
extern UnkStatics_7100434c90 g_unk_71726c7dd8 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726c7c70;
void fn_710042fd24(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_710042f998();
extern char g_unk_71726c7d88[];
extern UnkConversion g_unk_71726c7d90;
extern UnkConversion g_unk_71726c7db0;
void fn_71004336d8() __attribute__((visibility("hidden")));
void fn_71004336e4() __attribute__((visibility("hidden")));
extern char g_unk_71726c7dd0[];

void fn_7100434c90() {
    InitHeaderIds(&g_unk_71726c7dd8.ids);
    InitId(&g_unk_71726c7dd8.field_0x18, "fDamageChozoCommanderKiStrike");
    InitId(&g_unk_71726c7dd8.field_0x20, "attack_aurascratch");
    InitEnum(&g_unk_71726c7c70, "CChozoCommanderKiGrabAttack::SState", fn_710042fd24, fn_710042f998,
             g_unk_71726c7d88, &g_unk_71726c7d90, fn_71004336d8, &g_unk_71726c7db0, fn_71004336e4);
    fn_710042fea8();
    fn_710009ec00(g_unk_71726c7dd0, g_unk_71726c7dd8.field_0x40);
}
