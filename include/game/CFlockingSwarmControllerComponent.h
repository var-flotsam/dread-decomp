#pragma once

// CFlockingSwarmControllerComponent: functions of this translation unit that other files call (tools/tu_map.py unit 1158,
// 0x7100737af0-0x7100744c80). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkArray_fs;
struct UnkGroup_fs;
struct UnkParent_71007455dc;
struct UnkRedenkiSwarmController;
struct UnkSwarmPoint_b65a4;
struct UnkSwarmRecord_b65a4;
struct UnkSwarmUpdate_b65a4;
struct UnkVec3Result;
void fn_710073f7bc(UnkRedenkiSwarmController* self, u32 value);
UnkParent_71007455dc* fn_7100739c64(UnkSwarmUpdate_b65a4* self);
UnkGroup_fs* fn_71007410a4(UnkArray_fs* array, u32 index);
void fn_7100737d4c(UnkSwarmRecord_b65a4* record, const UnkSwarmPoint_b65a4* point);
bool fn_710073fea8(UnkSwarmUpdate_b65a4* self, const UnkVec3Result* pos, UnkSwarmPoint_b65a4* out, f32 range);
