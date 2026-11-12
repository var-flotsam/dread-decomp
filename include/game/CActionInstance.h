#pragma once

// CActionInstance: functions of this translation unit that other files call (tools/tu_map.py unit 2206,
// 0x7100c95b2c-0x7100c97f80). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "types.h"

struct UnkTrackAction_1444;
struct UnkPlayer_b42;
bool fn_7100c95ed8(UnkPlayer_b42*);
bool fn_7100c97930(UnkTrackAction_1444* action, const base::reflection::CClass* cls, f32 value);
