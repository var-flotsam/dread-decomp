// Static initialiser of the unit of the reflected types.
#include "../game_s7/UnkStaticInitS7.h"

using namespace unk_static_init;

struct UnkStatics_71005033d0 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    UnkFilePathStrIdBase field_0x28;
    UnkFilePathStrIdBase field_0x30;
    UnkStrIdBase field_0x38;
    u8 pad_0x40[0x8];
    f32 field_0x48[2];
    bool field_0x50;
    u8 pad_0x51[0x7];
    u8 field_0x58[8];
};
extern UnkStatics_71005033d0 g_unk_71726ddc28 __attribute__((visibility("hidden")));
extern char g_unk_71726ddc20[];
void* fn_71005022a4();  // CEmmyWaveMovementComponent::refl_GetClass

void fn_71005033d0() {
    InitHeaderIds(&g_unk_71726ddc28.ids);
    g_unk_71726ddc28.field_0x48[0] = -1.0f;
    g_unk_71726ddc28.field_0x48[1] = -1.0f;
    g_unk_71726ddc28.field_0x50 = true;
    InitId(&g_unk_71726ddc28.field_0x18, "damage");
    InitClassId(&g_unk_71726ddc28.field_0x20, "EmmyWaveMovementComponent");
    InitPathId(&g_unk_71726ddc28.field_0x28, "actors/weapons/emmywave/fx/emmywave_explosion.bcptl");
    InitPathId(&g_unk_71726ddc28.field_0x30, "actors/weapons/emmywave/fx/water_explosion.bcptl");
    InitId(&g_unk_71726ddc28.field_0x38, "emmywave_impact");
    fn_71005022a4();
    fn_710009ec00(g_unk_71726ddc20, g_unk_71726ddc28.field_0x58);
}
