// Static initialiser of the unit of the CShelmitPlasmaRayAttack::EAttackState enum.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71006b6e50 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    UnkFilePathStrIdBase field_0x28;
    UnkFilePathStrIdBase field_0x30;
    UnkFilePathStrIdBase field_0x38;
    u8 pad_0x40[0x8];
    u8 field_0x48[8];
};
extern UnkStatics_71006b6e50 g_unk_7172706920 __attribute__((visibility("hidden")));
extern char g_unk_71727067b8[];
extern UnkEnumType g_unk_71727067c0;
void fn_71006b36bc(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71006b31b0();
extern char g_unk_71727068d8[];
extern UnkConversion g_unk_71727068e0;
extern UnkConversion g_unk_7172706900;
void fn_71006b67c8() __attribute__((visibility("hidden")));
void fn_71006b67d4() __attribute__((visibility("hidden")));

void fn_71006b6e50() {
    InitHeaderIds(&g_unk_7172706920.ids);
    InitId(&g_unk_7172706920.field_0x18, "plasma_ray");
    InitId(&g_unk_7172706920.field_0x20, "Angle");
    InitPathId(&g_unk_7172706920.field_0x28, "actors/characters/shelmit/fx/shelmitimpactground.bcptl");
    InitPathId(&g_unk_7172706920.field_0x30, "actors/characters/shelmit/fx/shelmitimpactloop.bcptl");
    InitPathId(&g_unk_7172706920.field_0x38, "actors/characters/shelmit/fx/shelmit_destructionpath.bcptl");
    fn_71006b2f34();
    fn_710009ec00(g_unk_71727067b8, g_unk_7172706920.field_0x48);
    InitEnum(&g_unk_71727067c0, "CShelmitPlasmaRayAttack::EAttackState", fn_71006b36bc, fn_71006b31b0,
             g_unk_71727068d8, &g_unk_71727068e0, fn_71006b67c8, &g_unk_7172706900, fn_71006b67d4);
}
