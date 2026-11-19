#pragma once

// UnkTU372: functions of this translation unit that other files call (tools/tu_map.py unit 372,
// 0x71002b2e9c-0x71002b4934). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkQueryObj;
struct UnkNavMeshItem;
struct UnkLinkList;
struct UnkWeakTarget;
void* fn_71002b3b84(UnkWeakTarget*);
void fn_71002b46ec(UnkLinkList* list, UnkLinkList** slot);
void fn_71002b4538(UnkLinkList* list, UnkLinkList** slot);
void fn_71002b3114(UnkNavMeshItem* self, UnkNavMeshItem* other);
void fn_71002b3328(UnkNavMeshItem* self);
UnkQueryObj* fn_71002b37d8(void* tile);
