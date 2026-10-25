// Static initialiser of the unit of the reflected types.
#include "../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71004d7d80 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    u8 pad_0x28[0x8];
    u8 field_0x30[8];
};
extern UnkStatics_71004d7d80 g_unk_71726d8c58 __attribute__((visibility("hidden")));
extern char g_unk_71726d8c50[];

void fn_71004d7d80() {
    InitHeaderIds(&g_unk_71726d8c58.ids);
    InitClassId(&g_unk_71726d8c58.field_0x18, "CooldownXBossFireBallMovementComponent");
    InitId(&g_unk_71726d8c58.field_0x20, "fDamageCooldownXBossFireBall");
    fn_71004d70d4();
    fn_710009ec00(g_unk_71726d8c50, g_unk_71726d8c58.field_0x30);
}
