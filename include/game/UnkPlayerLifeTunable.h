#pragma once

// CPlayerLifeComponent::CTunablePlayerLifeComponent's data (0xd0 bytes; unit 2716 of tools/tu_map.py): the
// reflected members by offset (tools/gen_fields.py), behind its base, base::tunable::CTunable, which this
// layout does not name. The one instance is g_unk_7172a0a0a8 (game/CPlayerLifeComponent.h).
#include "types.h"

struct UnkPlayerLifeTunable {
    u8 base_0x0[0x24];  // base::tunable::CTunable
    f32 fVariaSuitDamageMult;  // 0x24
    f32 fGravitySuitDamageMult;  // 0x28
    f32 fHyperSuitDamageMult;  // 0x2c
    f32 fHardModeDamageMult;  // 0x30
    f32 fMorphImplulseDefaultDamage;  // 0x34
    f32 fOnDeathMusicsFade;  // 0x38
    f32 fSlomoInterpTimeDead;  // 0x3c
    f32 fSlomoInterpTimeRevive;  // 0x40
    f32 fPlayerMaxLifeOverride;  // 0x44
    bool bIgnoreImpact;  // 0x48
    f32 fEasyModeDamageMult;  // 0x4c
    f32 fEasyModeDamageMult_Corpius;  // 0x50
    f32 fEasyModeDamageMult_Kraid;  // 0x54
    f32 fEasyModeDamageMult_Drogyga;  // 0x58
    f32 fEasyModeDamageMult_Experiment57;  // 0x5c
    f32 fEasyModeDamageMult_RavenBeak;  // 0x60
    f32 fEasyModeDamageMult_EliteChozoSoldier;  // 0x64
    f32 fEasyModeDamageMult_ChozoSoldierGhavoran;  // 0x68
    f32 fEasyModeDamageMult_ChozoSoldierElun;  // 0x6c
    f32 fEasyModeDamageMult_ChozoSoldierArtaria;  // 0x70
    f32 fEasyModeDamageMult_ChozoSoldierHanubia;  // 0x74
    f32 fEasyModeDamageMult_ChozoRobotGhavoran;  // 0x78
    f32 fEasyModeDamageMult_ChozoRobotFerenia;  // 0x7c
    f32 fEasyModeDamageMult_ChozoRobotx2Ferenia;  // 0x80
    f32 fEasyModeDamageMult_ChozoRobotx2Burenia;  // 0x84
    f32 fEasyModeDamageMult_Escue;  // 0x88
    f32 fEasyModeDamageMult_Golzuna;  // 0x8c
    f32 fEasyModeDropAmountFactor_Life;  // 0x90
    f32 fEasyModeDropAmountFactor_LifeBig;  // 0x94
    f32 fEasyModeDropAmountFactor_Missile;  // 0x98
    f32 fEasyModeDropAmountFactor_MissileBig;  // 0x9c
    f32 fEasyModeDropAmountFactor_PowerBomb;  // 0xa0
    f32 fEasyModeDropAmountFactor_PowerBombBig;  // 0xa4
    f32 fEasyModeDropAmountFactor_MiniXLife;  // 0xa8
    f32 fEasyModeDropAmountFactor_MiniXMissile;  // 0xac
    f32 fEasyModeDropAmountFactor_XYellowLife;  // 0xb0
    f32 fEasyModeDropAmountFactor_XGreenMissile;  // 0xb4
    f32 fEasyModeDropAmountFactor_XRedLife;  // 0xb8
    f32 fEasyModeDropAmountFactor_XRedMissile;  // 0xbc
    f32 fEasyModeDropAmountFactor_XRedPowerBomb;  // 0xc0
    f32 fEasyModeDropAmountFactor_XOrangePowerBomb;  // 0xc4
    bool bEasyModeEnemyProjectileOneHit;  // 0xc8
    f32 fEasyModeKraidBouncingCreatureLifeFactor;  // 0xcc
};
static_assert(__builtin_offsetof(UnkPlayerLifeTunable, bEasyModeEnemyProjectileOneHit) == 0xc8, "layout");
static_assert(sizeof(UnkPlayerLifeTunable) == 0xd0, "UnkPlayerLifeTunable layout");
