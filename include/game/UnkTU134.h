#pragma once

// UnkTU134: functions of this translation unit that other files call (tools/tu_map.py unit 134,
// 0x710014699c-0x71001478bc). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkMissilePart;
struct UnkFxObject_ovb0041;
struct UnkSwitchObject;
void fn_7100146dac(UnkSwitchObject* self, bool enable);
void fn_7100146d84(UnkFxObject_ovb0041* fx, const UnkVec3f* position);
void fn_7100146d98(UnkFxObject_ovb0041* fx, const UnkVec3f* angle);
bool fn_7100146c50(UnkMissilePart* part, u32 a, void* data, u32 b);
