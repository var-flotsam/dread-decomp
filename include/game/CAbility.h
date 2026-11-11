#pragma once

// CAbility: functions and globals of this translation unit that other files use (tools/tu_map.py unit 427,
// 0x71002f8194-0x71002facc0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkAbilityC_A2_b69;
struct UnkOpticAbility_ovb0059;
struct UnkSelf;
struct UnkSmAbility3_A2_b63;
struct UnkSmLockon_A1_b64;
struct UnkWaveChild_A2_b72;
void fn_71002f8848(UnkWaveChild_A2_b72* child);
void fn_71002f9460(UnkSelf*, u32);
void fn_71002f9694(UnkAbilityC_A2_b69* ability, bool on);
void fn_71002fa070(UnkSmAbility3_A2_b63*);
void fn_71002f9f18(UnkSmLockon_A1_b64*, s32 kind);
void fn_71002fa068(UnkSmLockon_A1_b64* self);
void fn_71002fa2bc(UnkOpticAbility_ovb0059* self);
extern void* g_unk_7101a223f8[];  // CAbility's vtable.
