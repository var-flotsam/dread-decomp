#pragma once

// UnkTU341: functions of this translation unit that other files call (tools/tu_map.py unit 341,
// 0x710022739c-0x710024e2e8). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkActor_71002329e8;
struct UnkBoundObj;
struct UnkKeyMap_b60;
struct UnkKeyNode_b60;
struct UnkKeyedStrIdMapA_ovb0062;
struct UnkNameHolder_b0161;
struct UnkNodeMap_710025ad0c;
struct UnkNode_710025ad0c;
struct UnkPowerUpLifeEntry_b52;
struct UnkPowerUpLifeRef_b52;
struct UnkReflType;
struct UnkStruct_71002334fc;
struct UnkTreeNode;
void fn_7100232c68(void*);
void fn_71002334fc(UnkStruct_71002334fc*, bool);
void fn_7100232124(UnkBoundObj* obj);
UnkNameHolder_b0161* fn_7100232fcc(void* object);
void fn_71002486a8(UnkNodeMap_710025ad0c* map, UnkNode_710025ad0c* node);  // Removes a node.
void fn_710023272c(void* actor);
void fn_710023298c(UnkTreeNode* self);
void fn_71002329e8(UnkActor_71002329e8* self);
UnkPowerUpLifeRef_b52* fn_7100232f18(UnkPowerUpLifeEntry_b52* entry);
void fn_7100239e0c(char*);
void fn_710023ac4c(void* self);  // Re-points `current` at (index, element).
UnkReflType* fn_710023e498();  // base::reflection::refl_GetType<CActorComponent*>.
void fn_71002430e8(UnkKeyMap_b60* map, UnkKeyNode_b60* node);
void fn_7100246d70(void* self, const void* other);
void fn_7100248ce8(UnkKeyedStrIdMapA_ovb0062* self, u32 capacity);  // The keyed array's reserve.
