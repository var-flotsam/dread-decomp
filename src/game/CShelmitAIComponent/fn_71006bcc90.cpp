// Static initialiser of the unit of the EShellState enum.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71006bcc90 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    UnkStrIdBase field_0x38;
    UnkStrIdBase field_0x40;
    UnkStrIdBase field_0x48;
    UnkStrIdBase field_0x50;
    u8 pad_0x58[0x8];
    u8 field_0x60[8];
};
extern UnkStatics_71006bcc90 g_unk_7172707188 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_7172707020;
void fn_71006b973c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71006b95f0();
extern char g_unk_7172707138[];
extern UnkConversion g_unk_7172707140;
extern UnkConversion g_unk_7172707160;
void fn_71006bca54() __attribute__((visibility("hidden")));
void fn_71006bca60() __attribute__((visibility("hidden")));
extern char g_unk_7172707180[];

void fn_71006bcc90() {
    InitHeaderIds(&g_unk_7172707188.ids);
    InitId(&g_unk_7172707188.field_0x18, "Angle");
    InitId(&g_unk_7172707188.field_0x20, "Factor");
    InitId(&g_unk_7172707188.field_0x28, "unsheltered_relax");
    InitId(&g_unk_7172707188.field_0x30, "fDamageShelmitExplosion");
    InitId(&g_unk_7172707188.field_0x38, "fDamageShelmitDefault");
    InitId(&g_unk_7172707188.field_0x40, "fDamageShelmitNakedExplosion");
    InitId(&g_unk_7172707188.field_0x48, "fDamageShelmitNakedDefault");
    InitId(&g_unk_7172707188.field_0x50, "flee2patrol_walk");
    InitEnum(&g_unk_7172707020, "EShellState", fn_71006b973c, fn_71006b95f0,
             g_unk_7172707138, &g_unk_7172707140, fn_71006bca54, &g_unk_7172707160, fn_71006bca60);
    fn_71006b98c0();
    fn_710009ec00(g_unk_7172707180, g_unk_7172707188.field_0x60);
}
