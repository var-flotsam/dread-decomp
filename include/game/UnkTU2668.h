#pragma once

// UnkTU2668: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2668,
// 0x710109c64c-0x710109d670). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct lua_State;
struct UnkLuaScratchStack;

void fn_710109d0a0();
void fn_710109c9e0();  // Pops the scratch array the last run pushed (and frees it).
extern UnkLuaScratchStack g_unk_7172a025a0 __attribute__((visibility("hidden")));
extern lua_State* g_unk_7172a01d18;  // The game's Lua state.
s32 fn_710109cef0(lua_State* L, s32 nargs, s32 nresults, s32 errfunc);
