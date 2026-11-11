#pragma once

// UnkTU2677: functions of this translation unit that other files call (tools/tu_map.py unit 2677,
// 0x71010bee20-0x71010c0fec). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct lua_State;
s32 fn_71010c0270(lua_State* L, const char* fmt, ...);
int fn_71010c0b40(lua_State* L, const char* fmt, ...);
s32 fn_71010c0d80(lua_State* L, s32 a, char* buf, u32 size, void* x, void* y);
s32 fn_71010c0ad0(lua_State* L, int first, int count, const char* format, ...);
