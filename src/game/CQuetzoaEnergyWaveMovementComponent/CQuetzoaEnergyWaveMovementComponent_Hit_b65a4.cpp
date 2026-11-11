// CQuetzoaEnergyWaveMovementComponent's unit (tools/tu_map.py): a hit handler. Clears the speed at +0x7c
// and runs fn_7100ac38fc; unless the word at +0x5ec is set, a hit whose normal (+0x2c of the third
// argument) points up by less than 0.75 goes to the base handler fn_7100ac4c60, a steeper one to
// fn_710064cc30. Always returns true. Names and layouts are placeholders.
#include "game/CProjectileMovement.h"
#include "types.h"
#include "UnkLeftoverReg.h"

struct UnkEnergyWave_b65a4;

// As src/game/CKraidAcidBlobsMovementComponent/CKraidAcidBlobsMovementComponent.cpp declares it.
struct UnkBossComponent;

struct UnkEnergyWaveHit_b65a4 {
    u8 pad_0x0[0x2c];
    f32 nx, ny, nz;  // 0x2c
};

struct UnkEnergyWave_b65a4 {
    u8 pad_0x0[0x7c];
    f32 speed;  // 0x7c
    u8 pad_0x80[0x5ec - 0x80];
    u32 field_0x5ec;
};

// The second argument is unused (the original leaves x1 as it is).
struct UnkUnused_b65a4 : UnkLeftoverReg {};
void fn_710064cc30(UnkEnergyWave_b65a4* self, UnkUnused_b65a4, const UnkEnergyWaveHit_b65a4* hit);

bool fn_710064cb94(UnkEnergyWave_b65a4* self, u64 a, u64 b, UnkEnergyWaveHit_b65a4* hit, u64 d) {
    self->speed = 0.0f;
    fn_7100ac38fc(self);
    if (self->field_0x5ec == 0) {
        if (hit->nx * 0.0f + hit->ny + hit->nz * 0.0f >= 0.75f)
            fn_710064cc30(self, UnkUnused_b65a4(), hit);
        else
            fn_7100ac4c60(reinterpret_cast<UnkBossComponent*>(self), a, b, reinterpret_cast<u64>(hit), d);
    }
    return true;
}
