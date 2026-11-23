#pragma once

// CDoorLifeComponent::CTunableDoorLifeComponent's data (0x38 bytes), the tunable g_unk_7172731368. Members and
// offsets are reflection's (tools/gen_fields.py); bForceAllDoorsOpened is reflected without an offset and sits in
// the gap at 0x2c, where the door's update (src/game/UnkTU1376) reads it. The type keeps a placeholder name
// because the generated reflection uses the class's name as a namespace. Its static initialiser is
// fn_71008da6c0 (src/game_s7).
#include "game/UnkTUProperty.h"
#include "types.h"

struct UnkDoorLifeTunable : UnkTunable {
    u8 pad_0x8[0x1c];  // the rest of base::tunable::CTunable (sName at 0x10)
    f32 fPhaseDisplacementFactorOverride;  // 0x24
    f32 fOneWayDoorCloseDistance;          // 0x28
    bool bForceAllDoorsOpened;             // 0x2c
    f32 fIceBlockOffSet;                   // 0x30
    u8 pad_0x34[0x4];
};
static_assert(__builtin_offsetof(UnkDoorLifeTunable, fPhaseDisplacementFactorOverride) == 0x24, "layout");
static_assert(__builtin_offsetof(UnkDoorLifeTunable, bForceAllDoorsOpened) == 0x2c, "layout");
static_assert(__builtin_offsetof(UnkDoorLifeTunable, fIceBlockOffSet) == 0x30, "layout");
static_assert(sizeof(UnkDoorLifeTunable) == 0x38, "layout");

extern UnkDoorLifeTunable g_unk_7172731368;
