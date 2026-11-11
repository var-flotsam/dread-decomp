#pragma once

// UnkTU2510: functions of this translation unit that other files call (tools/tu_map.py unit 2510,
// 0x7100d5761c-0x7100d5d23c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkBox;
struct UnkColliderNode_A6_b70;
struct UnkMtx44;
struct UnkShape;
struct UnkStruct_7100861148_Collider;
struct UnkTree;
bool fn_7100d5b484(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, void* arg, u16 arg2);
void fn_7100d5ac50(void* owner, UnkStruct_7100861148_Collider* group);
bool fn_7100d5c518(UnkTree* self, u32 mask, const UnkBox* box, void* arg, u32 arg2);
void fn_7100d5c888(void* owner, UnkColliderNode_A6_b70* collider, u32 old_mask, u32 new_mask);
