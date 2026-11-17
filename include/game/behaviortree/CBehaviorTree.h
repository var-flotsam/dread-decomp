#pragma once

// CBehaviorTree: functions of this translation unit that other files call (tools/tu_map.py unit 548,
// 0x71003a9710-0x71003ab520). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkStruct_71003aad48;
struct UnkInverterTarget_b66a2;
struct UnkInverter_b66a2;
struct UnkSelectorNode_A2_b68;
struct UnkSelectorTree_A2_b68;
// Queues `node` at the front of the tree's pending list in state 1; false when it is pending already.
bool fn_71003aab64(UnkSelectorTree_A2_b68* tree, UnkSelectorNode_A2_b68* node);
// Replaces the pending entry for `node` with one in `status`, at the front; false when there is none.
bool fn_71003aac0c(UnkInverterTarget_b66a2* target, UnkInverter_b66a2* node, s32 status);
bool fn_71003aad48(UnkStruct_71003aad48* self, void* key);
