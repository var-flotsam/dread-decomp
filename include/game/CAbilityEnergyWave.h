#pragma once

// CAbilityEnergyWave: functions and globals of this translation unit that other files use (tools/tu_map.py unit 429,
// 0x71002fd4b0-0x7100306910). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkWeakTarget;
struct UnkEmmyRegistry_b55;
struct UnkAbility;
struct UnkAbilitySet;
struct UnkHitFlags_b152;
struct UnkParent;
struct UnkScriptCaller;
struct UnkStruct_71002fda50;
bool fn_7100305088(UnkScriptCaller* self, const char* name, const char* fmt, ...);
void fn_71002fda50(UnkStruct_71002fda50* self, bool all, bool force_off);
UnkAbility* fn_71003013ec(UnkAbilitySet* set, u64 id);
UnkAbility* fn_71003012f8(UnkAbilitySet* set, u64 id);
void fn_71002fe318(UnkParent*);
UnkHitFlags_b152 fn_7100301c0c(void*, void*, void*);
extern void* g_unk_71726af0b0;  // What plays the sounds at a position.
extern UnkEmmyRegistry_b55* g_unk_71726af0a8;
extern void* g_unk_7101a22858[];
extern void* g_unk_7101a22878[];
UnkWeakTarget* fn_7100302014(void* owner);
UnkWeakTarget* fn_7100303054(void* owner);
