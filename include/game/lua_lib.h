#pragma once

// lua_lib: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2618,
// 0x7100fbd320-0x7100fc17d0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "types.h"

struct UnkGameObjectTyped_ovb0115;
struct UnkRcBinding_b0163;
struct lua_State;
s32 fn_7100fbd320(lua_State* L, UnkRcBinding_b0163* binding, u32 a, u32 b);
extern void* g_unk_7101cab638[] __attribute__((visibility("hidden")));
int fn_7100fbc4e0(lua_State* L, void* arg);
int fn_7100fbe7b0(lua_State* L, UnkGameObjectTyped_ovb0115* value, base::global::CStrId* name);
const base::reflection::CType* fn_7100fc036c();
