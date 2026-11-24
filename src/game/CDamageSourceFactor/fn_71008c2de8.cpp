// Returns the multiplier for a set of damage-type flags from a table of factors: the first
// flag tested that is set selects its factor (some have a variant for flag 0x100 when one of
// the flags 0x400 / 0x8000000 is also set); 1.0 when none applies.
#include "game/CDamageSourceFactor.h"
#include "types.h"

struct UnkStruct_71008c2de8 {
    u8 pad_0x0[0x8];
    f32 fPowerBeamFactor;
    f32 fWideBeamFactor;
    f32 fPlasmaBeamFactor;
    f32 fWaveBeamFactor;
    f32 fGrappleBeamFactor;
    f32 fHyperBeamFactor;
    f32 fChargePowerBeamFactor;
    f32 fChargeWideBeamFactor;
    f32 fChargePlasmaBeamFactor;
    f32 fChargeWaveBeamFactor;
    f32 fMeleeChargePowerBeamFactor;
    f32 fMeleeChargeWideBeamFactor;
    f32 fMeleeChargePlasmaBeamFactor;
    f32 fMeleeChargeWaveBeamFactor;
    f32 fMissileFactor;
    f32 fSuperMissileFactor;
    f32 fIceMissileFactor;
    f32 fMultiLockonMissileFactor;
    f32 fBombFactor;
    f32 fLineBombFactor;
    f32 fPowerBombFactor;
    f32 fScrewAttackFactor;
    f32 fDashMeleeFactor;
    f32 fSpeedBoosterFactor;
    f32 fShineSparkFactor;
};

f32 fn_71008c2de8(const UnkStruct_71008c2de8* self, u32 flags) {
    if (flags & 0x10000)
        return self->fHyperBeamFactor;
    if (flags & 0x8000400) {
        bool alt = (flags & 0x100) != 0;
        if (flags & 0x1000)
            return alt ? self->fMeleeChargeWaveBeamFactor : self->fChargeWaveBeamFactor;
        if (flags & 0x4000)
            return alt ? self->fMeleeChargePlasmaBeamFactor : self->fChargePlasmaBeamFactor;
        if (flags & 0x2000)
            return alt ? self->fMeleeChargeWideBeamFactor : self->fChargeWideBeamFactor;
        if (flags & 0x1)
            return alt ? self->fMeleeChargePowerBeamFactor : self->fChargePowerBeamFactor;
        return 1.0f;
    }
    if (flags & 0x1000)
        return self->fWaveBeamFactor;
    if (flags & 0x4000)
        return self->fPlasmaBeamFactor;
    if (flags & 0x2000)
        return self->fWideBeamFactor;
    if (flags & 0x1)
        return self->fPowerBeamFactor;
    if (flags & 0x200000)
        return self->fGrappleBeamFactor;
    if (flags & 0x200)
        return self->fMultiLockonMissileFactor;
    if (flags & 0x2)
        return self->fIceMissileFactor;
    if (flags & 0x8)
        return self->fSuperMissileFactor;
    if (flags & 0x4)
        return self->fMissileFactor;
    if (flags & 0x20)
        return self->fPowerBombFactor;
    if (flags & 0x20000000)
        return self->fLineBombFactor;
    if (flags & 0x10)
        return self->fBombFactor;
    if (flags & 0x8000)
        return self->fShineSparkFactor;
    if (flags & 0x2000000)
        return self->fSpeedBoosterFactor;
    if (flags & 0x40000)
        return self->fDashMeleeFactor;
    if (flags & 0x40)
        return self->fScrewAttackFactor;
    return 1.0f;
}
