#pragma once

// UnkTU244: functions of this translation unit that other files call (tools/tu_map.py unit 244,
// 0x71001b76ec-0x71001bb60c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkXformTarget;
struct UnkCullView;
void fn_71001b76ec(UnkCullView* self);
void fn_71001b7a60(UnkCullView* self);
void fn_71001ba980(UnkXformTarget* target, const UnkVec3f* pos);
void fn_71001ba864(UnkXformTarget* target, const UnkVec3f* pos);
