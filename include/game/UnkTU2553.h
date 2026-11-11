#pragma once

// UnkTU2553: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2553,
// 0x7100de277c-0x7100e04d68). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkScriptManager_ovb0085;
struct UnkObj_7100e01680;
struct UnkPlayer;
struct UnkStepCallback;
struct UnkStepCallbacks;
bool fn_7100df5874(UnkPlayer* self);
void fn_7100def888(char* p0);
void fn_7100df492c(void* self, void* a, const void* b, void* c);
void fn_7100df7468();
UnkStepCallback* fn_7100dfc428(UnkStepCallbacks* self, u32 index);
void* fn_7100e01680(UnkObj_7100e01680* self);
extern u32 g_unk_71727bebb8;  // The kind the main camera is looked up with.
extern UnkScriptManager_ovb0085* g_unk_71727bed90;
