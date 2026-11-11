#pragma once

// UnkTU1176: functions of this translation unit that other files call (tools/tu_map.py unit 1176,
// 0x710076cf54-0x710077c5ec). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkCentralUnitList;
struct UnkCentralUnitProto;
struct UnkCentralUnit_b46a2;
struct UnkProtoCentralUnitBase;
struct UnkScenarioObject_ovb0032;
UnkCentralUnitList* fn_7100772c98();
void* fn_7100774014(UnkScenarioObject_ovb0032* actor);
void fn_710076e964(UnkProtoCentralUnitBase* self);
void fn_71007722f4(UnkCentralUnit_b46a2* self, f32 value);
void fn_710077561c(UnkCentralUnitProto* unit);
