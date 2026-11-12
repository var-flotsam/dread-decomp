#pragma once

// UnkTU133: functions and globals of this translation unit that other files use (tools/tu_map.py unit 133,
// 0x7100145bdc-0x710014699c). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkSwitchObject;

extern char g_unk_7101a1c7d8[];
void fn_7100145f28(UnkSwitchObject* self, const UnkVec3f* position);
void fn_7100145f3c(UnkSwitchObject* self, const UnkVec3f* angles);
