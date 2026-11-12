#pragma once

// UnkTU2676: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2676,
// 0x71010a0880-0x71010bee20). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct lua_State;
int fn_71010a4900(lua_State* L, int narg);

// The userdata at `index` when its metatable is the registry's `name`, else null.
void* fn_71010a4740(lua_State* L, int index, const char* name);
// The error printer the game may replace (reached through the GOT); fn_71010a5a70 until then.
extern int (*g_unk_7101caf838)(const char* fmt, ...);
