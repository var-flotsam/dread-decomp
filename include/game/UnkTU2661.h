#pragma once

// UnkTU2661: functions of this translation unit that other files call (tools/tu_map.py unit 2661,
// 0x71010626f0-0x7101073b80). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkCutsceneObject_cs;
struct UnkVec3_cs;
u32 fn_7101068c80(UnkCutsceneObject_cs* object, void* entry);
void fn_710106bcfc(UnkCutsceneObject_cs* object, const UnkVec3_cs& pos);
