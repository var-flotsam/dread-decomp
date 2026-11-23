#pragma once

#include "game/UnkTeleporterFx_b0233.h"

struct UnkTeleporterSelf_b0233 {
    u8 pad_0x0[0x30];
    UnkTeleporterFx_b0233* actor;  // 0x30
    u8 pad_0x38[0x550];
    u32 mode;  // 0x588
};
