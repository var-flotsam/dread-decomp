#pragma once

// CInputComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1326,
// 0x710087043c-0x71008748b8). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkWeakTarget;
struct UnkEnergyWaveTargetComp_b49;
struct UnkInputComponent;
void fn_7100872460(UnkInputComponent* self, bool enable, bool reset, void* arg);
bool fn_710087238c(UnkInputComponent*);
bool fn_7100872860(UnkEnergyWaveTargetComp_b49* comp, void* link);
void fn_7100872268(UnkInputComponent* self);
void fn_7100872858(void* p, void* q);
extern void* g_unk_7172729910;  // The item list singleton.
void fn_7100874460(void* list, UnkWeakTarget** item);
void fn_7100872908(void*, void*);
