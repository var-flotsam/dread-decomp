#pragma once

// CTunableRumbleManager: functions and globals of this translation unit that other files use (tools/tu_map.py unit 51,
// 0x7100072564-0x7100075db8). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/input/UnkRumbleManager.h"
#include "types.h"
void fn_7100075104(void* self);  // A destructor (its callers register it).
extern void* g_unk_7101a1a5a8[];
void fn_7100074e3c(base::input::UnkRumbleManager* self, bool enabled);
void fn_7100074f94(base::input::UnkRumbleManager* self, const u32* mask);
void fn_7100073f48(base::input::UnkRumbleManager* self, u32 a, u32 b);
bool fn_710007453c(base::input::UnkRumbleManager* self);
