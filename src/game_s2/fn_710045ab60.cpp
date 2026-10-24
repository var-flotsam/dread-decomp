// Static initialiser of the unit of the reflected types.
#include "../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_710045ab60 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    u8 pad_0x30[8];
    f32 field_0x38[2];
    bool field_0x40;
    u8 pad_0x41[7];
    u8 field_0x48[8];
};
extern UnkStatics_710045ab60 g_unk_71726c9de0 __attribute__((visibility("hidden")));
extern char g_unk_71726c9dd8[];

void fn_710045ab60() {
    InitHeaderIds(&g_unk_71726c9de0.ids);
    g_unk_71726c9de0.field_0x38[0] = -1.0f;
    g_unk_71726c9de0.field_0x38[1] = -1.0f;
    g_unk_71726c9de0.field_0x40 = true;
    InitId(&g_unk_71726c9de0.field_0x18, "chozocommanderenergyshardsfragment");
    InitId(&g_unk_71726c9de0.field_0x20, "fDamageChozoCommanderEnergyShardsSphere");
    InitClassId(&g_unk_71726c9de0.field_0x28, "CChozoCommanderEnergyShardsSphereMovementComponent");
    fn_7100459b48();
    fn_710009ec00(g_unk_71726c9dd8, g_unk_71726c9de0.field_0x48);
}
