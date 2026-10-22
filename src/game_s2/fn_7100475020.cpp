// Static initialiser of the unit of the reflected types.
#include "../game_s7/UnkStaticInitS7.h"

using namespace unk_static_init;

struct UnkStatics_7100475020 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    UnkStrIdBase field_0x38;
    u8 pad_0x40[0x28];
    u8 field_0x68[8];
};
extern UnkStatics_7100475020 g_unk_71726cc9e8 __attribute__((visibility("hidden")));
void* fn_710046ea2c();  // CChozoRobotSoldierAIComponent::refl_GetClass
extern char g_unk_71726cc9e0[];

void fn_7100475020() {
    InitHeaderIds(&g_unk_71726cc9e8.ids);
    InitClassId(&g_unk_71726cc9e8.field_0x18, "CChozoRobotSoldierAIComponent");
    InitId(&g_unk_71726cc9e8.field_0x20, "shockwave");
    InitId(&g_unk_71726cc9e8.field_0x28, "fDamageChozoRobotSoldierExplosion");
    InitId(&g_unk_71726cc9e8.field_0x30, "fDamageChozoRobotSoldierExplosionElite");
    InitId(&g_unk_71726cc9e8.field_0x38, "SP_ChozoRobotSoldier_chozorobotsoldier");
    fn_710046ea2c();
    fn_710009ec00(g_unk_71726cc9e0, g_unk_71726cc9e8.field_0x68);
}
