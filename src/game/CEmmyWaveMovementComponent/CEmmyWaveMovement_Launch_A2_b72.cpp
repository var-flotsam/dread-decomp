// CEmmyWaveMovementComponent's unit (tools/tu_map.py unit 796): launch set-up of the wave's movement
// component (second argument), as CCooldownXBossFireBallMovement_Launch_A2_b70.cpp: zero damage value
// (fn_71008c1538), a shared string id into +0xb8, flags, the unit's static string id into +0xa0, bit 26 of
// the flags at +0xb4, mode 2, and a horizontal direction that is the sign of the owner's +0x40.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU1370.h"
#include "types.h"

using base::global::CStrId;

struct UnkRequest;
extern CStrId g_unk_71726dda50;
extern CStrId g_unk_71726ddc60 __attribute__((visibility("hidden")));

struct UnkVec2_A2_b72 {
    f32 x;
    f32 y;
};

struct UnkEmmyWaveMover_A2_b72 {
    u8 pad_0x0[0x18];
    bool field_0x18;
    u8 pad_0x19[0x3];
    u32 field_0x1c;
    u8 pad_0x20[0x3];
    bool field_0x23;
    u8 pad_0x24[0x25];
    bool field_0x49;
    u8 pad_0x4a[0x2];
    UnkVec2_A2_b72 field_0x4c;
    u8 pad_0x54[0x4c];
    CStrId field_0xa0;
    u8 pad_0xa8[0xc];
    u32 flags;  // 0xb4
    CStrId id;  // 0xb8
};

struct UnkEmmyWave_A2_b72 {
    u8 pad_0x0[0x40];
    f32 field_0x40;
};

void fn_710050315c(UnkEmmyWave_A2_b72* self, UnkEmmyWaveMover_A2_b72* mover) {
    fn_71008c1538(reinterpret_cast<UnkRequest*>(mover), 0.0f);
    mover->id = g_unk_71726dda50;
    mover->field_0x18 = true;
    mover->field_0x23 = true;
    mover->field_0xa0 = g_unk_71726ddc60;
    mover->flags |= 0x4000000;
    mover->field_0x1c = 2;
    mover->field_0x49 = true;
    f32 x = self->field_0x40;
    mover->field_0x4c = UnkVec2_A2_b72{static_cast<f32>((x > 0.0f) - (x < 0.0f)), 1.0f};
}
