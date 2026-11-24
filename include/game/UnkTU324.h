#pragma once

// UnkTU324: functions of this translation unit that other files call (tools/tu_map.py unit 324,
// 0x710021e6e8-0x71002206fc). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkMedia324;
struct UnkMediaInstance;
struct UnkRefObj;
struct UnkRefObj_7100f011c4;
struct UnkRefObj_7100f00f4c;
void fn_710021ea60(UnkRefObj_7100f00f4c* self);
bool fn_710021eec0(UnkMediaInstance* instance, s32 state);  // Whether an instance is in `state`.
u32 fn_710021f950(UnkMedia324* self);
// Sets the state (when not negative) and tells the listeners unless it is 1; whether it told them.
bool fn_710021f168(UnkMedia324* self, s32 state);
void fn_710021fafc(UnkRefObj_7100f011c4* self);
void fn_71002204d8(void* table, u32 count);

// Virtual tables of the unit (their entries start at index 2).
extern void* g_unk_7101a20498[];
extern void* g_unk_7101a204c0[];
