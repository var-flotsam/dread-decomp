#pragma once

// CBreakableTileGroup: functions of this translation unit that other files call (tools/tu_map.py unit 2213,
// 0x7100c9da0c-0x7100ca3890). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkTileQuery_A3_b64;
struct UnkTileGroup;
struct UnkObserverEntry_A6_b68;
struct UnkQueryObj;
u32 fn_7100c9e960(UnkQueryObj* obj);
UnkQueryObj* fn_7100ca2048(void* observer, void* query, u32 flags);
void fn_7100c9e6ac(UnkQueryObj* obj, u32 state, bool a, bool b);
void fn_7100c9ea08(UnkQueryObj* obj);
void fn_7100ca3498(UnkObserverEntry_A6_b68* entry);
UnkTileQuery_A3_b64* fn_7100ca1ecc(base::reflection::UnkReflObject* group, const void* at, void* arg, bool flag);
void fn_7100ca0c40(UnkTileGroup* self, u32 tile, UnkVec3f* pos);
void fn_7100ca0c78(UnkTileGroup* self, u32 tile, UnkVec3f* pos, UnkVec3f* scale);
