// Constructor of a component: runs the base constructor, then constructs seventeen members in place
// (each by its own constructor) and sets a few defaults (four pairs of tuning floats and two flags).
//
// The two flag bytes at +0x4fc and +0x4fd (one u16 member of the base) are addressed by offset.
#include "game/CDamageSourceFactor.h"
#include "game/UnkBossAIComponent.h"
#include "types.h"

extern void* g_unk_7101a369a8[];

// The members after the base, constructed in place by their own constructors.
struct UnkComponent_44e048 : UnkBossAIComponent {
    u8 oChozoCommanderTriComboAttackDef[0x100];  // 0x528
    u8 oChozoCommanderKiStrikeAttackDef[0x148];  // 0x628
    u8 oChozoCommanderSentenceSphereAttackDef[0x100];  // 0x770
    u8 oChozoCommanderPowerPulseAttackDef[0x118];  // 0x870
    u8 oChozoCommanderAuraScratchAttackDef[0x100];  // 0x988
    u8 oChozoCommanderKiCounterAttackDef[0x110];  // 0xa88
    u8 oChozoCommanderLandingSlamAttackDef[0x108];  // 0xb98
    u8 oChozoCommanderAirChargeAttackDef[0xf0];  // 0xca0
    u8 oChozoCommanderZeroLaserAttackDef[0x118];  // 0xd90
    u8 oChozoCommanderBeamBurstAttackDef[0xf0];  // 0xea8
    u8 oChozoCommanderHypersparkAttackDef[0x108];  // 0xf98
    u8 oChozoCommanderEnergyShardsAttackDef[0x108];  // 0x10a0
    u8 oChozoCommanderZeroLaserGroundedAttackDef[0x110];  // 0x11a8
    u8 oChozoCommanderHyperDashAttackDef[0x108];  // 0x12b8
    u8 oChozoCommanderUltimateGrabAttackDef[0xf0];  // 0x13c0
    u8 oChozoCommanderKiGrabAttackDef[0xf0];  // 0x14b0
    f32 fAuraLife;
    f32 fLifeToRecoverAfterUltimateGrabStage1;
    f32 fLifeToRecoverAfterUltimateGrabStage3;
    f32 fUltimateGrabDamage;
    f32 fTimeBetweenKiStrikeAttacks;
    f32 fAttackWaitTimeBetweenPhases;
    f32 fMinDistToHyperDash;
    f32 fMaxVerticalDiffToAboveHyperDash;
    u8 oDamageSourceFactorShortShootingGrab[0x70];  // 0x15c0
    u8 oDamageSourceFactorLongShootingGrab[0x70];  // 0x1630: the same kind as +0x15c0 (fn_71008c2da8)
};

void fn_7100422d7c(void* member);
void fn_7100420fa4(void* member);
void fn_71004228f8(void* member);
void fn_7100422200(void* member);
void fn_710041d07c(void* member);
void fn_71004202c8(void* member);
void fn_7100421740(void* member);
void fn_710041c440(void* member);
void fn_7100423b2c(void* member);
void fn_710041e310(void* member);
void fn_710041fb50(void* member);
void fn_710041e990(void* member);
void fn_7100424690(void* member);
void fn_710041f538(void* member);
void fn_710042315c(void* member);
void fn_71004206fc(void* member);

template <class T>
static inline T& At(void* base, u64 offset) {
    return *reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
}

void fn_710044e048(UnkComponent_44e048* self) {
    fn_710041a198(self);
    self->vtable = g_unk_7101a369a8 + 2;
    fn_7100422d7c(self->oChozoCommanderTriComboAttackDef);
    fn_7100420fa4(self->oChozoCommanderKiStrikeAttackDef);
    fn_71004228f8(self->oChozoCommanderSentenceSphereAttackDef);
    fn_7100422200(self->oChozoCommanderPowerPulseAttackDef);
    fn_710041d07c(self->oChozoCommanderAuraScratchAttackDef);
    fn_71004202c8(self->oChozoCommanderKiCounterAttackDef);
    fn_7100421740(self->oChozoCommanderLandingSlamAttackDef);
    fn_710041c440(self->oChozoCommanderAirChargeAttackDef);
    fn_7100423b2c(self->oChozoCommanderZeroLaserAttackDef);
    fn_710041e310(self->oChozoCommanderBeamBurstAttackDef);
    fn_710041fb50(self->oChozoCommanderHypersparkAttackDef);
    fn_710041e990(self->oChozoCommanderEnergyShardsAttackDef);
    fn_7100424690(self->oChozoCommanderZeroLaserGroundedAttackDef);
    fn_710041f538(self->oChozoCommanderHyperDashAttackDef);
    fn_710042315c(self->oChozoCommanderUltimateGrabAttackDef);
    fn_71004206fc(self->oChozoCommanderKiGrabAttackDef);
    self->fAuraLife = 1000.0f;
    self->fLifeToRecoverAfterUltimateGrabStage1 = 500.0f;
    self->fLifeToRecoverAfterUltimateGrabStage3 = 1000.0f;
    self->fUltimateGrabDamage = 100.0f;
    self->fTimeBetweenKiStrikeAttacks = 20.0f;
    self->fAttackWaitTimeBetweenPhases = 0.6f;
    self->fMinDistToHyperDash = 250.0f;
    self->fMaxVerticalDiffToAboveHyperDash = 200.0f;
    fn_71008c2da8(self->oDamageSourceFactorShortShootingGrab);
    fn_71008c2da8(self->oDamageSourceFactorLongShootingGrab);
    self->field_0x409 = 0;
    At<u8>(self, 0x4fc) = 1;
    At<u8>(self, 0x4fd) = 1;
}
