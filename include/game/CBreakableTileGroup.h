#pragma once

// CBreakableTileGroup: functions of this translation unit that other files call (tools/tu_map.py unit 2213,
// 0x7100c9da0c-0x7100ca3890). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkObserverEntry_A6_b68;
struct UnkQueryObj;
u32 fn_7100c9e960(UnkQueryObj* obj);
UnkQueryObj* fn_7100ca2048(void* observer, void* query, u32 flags);
void fn_7100c9e6ac(UnkQueryObj* obj, u32 state, bool a, bool b);
void fn_7100c9ea08(UnkQueryObj* obj);
void fn_7100ca3498(UnkObserverEntry_A6_b68* entry);
