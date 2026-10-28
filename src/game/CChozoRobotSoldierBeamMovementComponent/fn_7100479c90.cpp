// Static initialiser of the unit of the reflected types.
#include "../../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_7100479c90 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    UnkStrIdBase field_0x38;
    u8 pad_0x40[0x8];
    u8 field_0x48[8];
};
extern UnkStatics_7100479c90 g_unk_71726cd548 __attribute__((visibility("hidden")));
extern char g_unk_71726cd540[];

void fn_7100479c90() {
    InitHeaderIds(&g_unk_71726cd548.ids);
    InitClassId(&g_unk_71726cd548.field_0x18, "ChozoRobotSoldierBeamMovementComponent");
    InitId(&g_unk_71726cd548.field_0x20, "fDamageChozoRobotSoldierCannonShot");
    InitId(&g_unk_71726cd548.field_0x28, "fDamageChozoRobotSoldierCannonShotCharged");
    InitId(&g_unk_71726cd548.field_0x30, "fDamageChozoRobotSoldierCannonShotElite");
    InitId(&g_unk_71726cd548.field_0x38, "fDamageChozoRobotSoldierCannonShotChargedElite");
    fn_7100478eb4();
    fn_710009ec00(g_unk_71726cd540, g_unk_71726cd548.field_0x48);
}
