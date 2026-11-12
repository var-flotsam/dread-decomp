#pragma once

// UnkTU2620: functions of this translation unit that other files call (tools/tu_map.py unit 2620,
// 0x7100fc22a0-0x7100fc350c). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/reflection/CClass.h"
#include "types.h"

struct UnkLuaReadScratch_b49a6;
struct lua_State;
struct UnkParseContext;
bool fn_7100fc22a0(lua_State* L, s32 index, base::reflection::UnkTypedRef* value, u32 mode, UnkLuaReadScratch_b49a6* scratch);
s32 fn_7100fc2700(UnkParseContext* self, const base::reflection::CType* type, void* x2);
