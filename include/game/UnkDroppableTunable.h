#pragma once

// CLogicDroppedItemManager::CTunableDroppableBillboard's data (0x258 bytes), the tunable g_unk_71727c10c8
// (game/CLogicDroppedItemManager/CTunableDroppableBillboard.h). Members and offsets are reflection's
// (tools/gen_fields.py); the type keeps a placeholder name because the generated reflection uses the class's name
// as a namespace. Its constructor and destructors are in src/game/UnkTU2562/UnkTU2562_Billboard_ovb0208.cpp, its
// vtable is g_unk_7101aa9cc8 (game/UnkTU2562.h).
#include "base/global/CRntString.h"
#include "game/UnkTUProperty.h"
#include "types.h"

struct UnkDroppableTunable : UnkTunable {
    u8 pad_0x8[0x1c];  // the rest of base::tunable::CTunable (sName at 0x10)
    f32 fSpawnSpeedFactor;  // 0x24
    f32 fMaxSpawnVelocity;
    f32 fMaxTimeAlive;
    f32 fMiniXMaxTimeAlive;  // 0x30
    f32 fTimeToStartBlinking;
    f32 fMiniXTimeToStartBlinking;
    f32 fBlinkPeriod;
    f32 fMaxSqrDistanceToAbsorb;  // 0x40
    f32 fLifeAmount;
    f32 fLifeBigAmount;
    f32 fMissileAmount;
    f32 fMissileBigAmount;  // 0x50
    f32 fPowerBombAmount;
    f32 fPowerBombBigAmount;
    f32 fMiniXLifeAmount;
    f32 fMiniXMissileAmount;  // 0x60
    f32 fTimeSpawning;
    f32 fTimeSpawningMelee;
    f32 fTimeToBeCollectible;
    f32 fDropsRelaxMovementSize;  // 0x70
    s32 iDropsRelaxMovementAnimationFrames;
    bool bDropsDoRelaxMovement;  // 0x78; the billboards also index g_unk_71727c1088's ids with it
    bool bAbsorbDissolvingPickables;
    f32 fMiniXGreenMotionScale;  // 0x7c
    f32 fMiniXYellowMotionScale;
    f32 fMiniXScaleMinFactor;
    f32 fMiniXScaleMaxFactor;
    f32 fMiniXScaleFrequency;  // 0x8c
    u32 uMiniXSmallGroupMaxSize;
    u32 uMiniXMediumGroupMaxSize;
    base::global::CRntString sMiniXAbsorbSmallGroupPresetName;  // 0x98
    base::global::CRntString sMiniXAbsorbMediumGroupPresetName;
    base::global::CRntString sMiniXAbsorbLargeGroupPresetName;
    base::global::CRntString sMiniXFlyPresetName;  // 0x128
    base::global::CRntString sDropAbsorbLifePresetName;
    base::global::CRntString sDropAbsorbLifeBigPresetName;
    base::global::CRntString sDropAbsorbMissilePresetName;
    base::global::CRntString sDropAbsorbPowerBombPresetName;
    base::global::CRntString sAbsorbingSoundPresetName;  // 0x218
    bool bMissileBigDeprecated;  // 0x248
    bool bPowerBombBigDeprecated;
    // Reflected without an offset, through a getter and a setter each: fn_7100e2f79c and fn_7100e2f894 read
    // these (src/clones), fn_7100e2f7e8 and fn_7100e2f8e0 write them.
    f32 fDefaultAttractionDistanceCm;  // 0x24c
    f32 fMiniXAttractionDistanceCm;    // 0x250
    u8 pad_0x254[0x4];
};
static_assert(__builtin_offsetof(UnkDroppableTunable, fSpawnSpeedFactor) == 0x24, "layout");
static_assert(__builtin_offsetof(UnkDroppableTunable, uMiniXMediumGroupMaxSize) == 0x94, "layout");
static_assert(__builtin_offsetof(UnkDroppableTunable, sAbsorbingSoundPresetName) == 0x218, "layout");
static_assert(__builtin_offsetof(UnkDroppableTunable, fDefaultAttractionDistanceCm) == 0x24c, "layout");
static_assert(sizeof(UnkDroppableTunable) == 0x258, "layout");
