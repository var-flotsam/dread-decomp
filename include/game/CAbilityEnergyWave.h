#pragma once

// CAbilityEnergyWave: functions and globals of this translation unit that other files use (tools/tu_map.py unit 429,
// 0x71002fd4b0-0x7100306910). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct CAbilityComponent;
struct CAbility;  // game/CAbilityLayout.h
struct CAbilityComponent;  // game/CAbilityLayout.h
struct UnkWeakHandle;
struct UnkWeakTarget;
struct UnkEmmyRegistry_b55;
struct UnkAbility;
struct UnkAbilitySet;
struct UnkHitFlags_b152;
struct UnkScriptCaller;
bool fn_7100305088(UnkScriptCaller* self, const char* name, const char* fmt, ...);
void fn_71002fda50(CAbilityComponent* self, bool all, bool force_off);
UnkAbility* fn_71003013ec(UnkAbilitySet* set, u64 id);
UnkAbility* fn_71003012f8(UnkAbilitySet* set, u64 id);
void fn_71002fe318(CAbilityComponent* self);
// Hands the component's life component the amount, clamped to what it has.
void CAbilityEnergyWave_ApplyClampedAmountToHandleTarget(CAbilityComponent* self, f32 amount);
// Tells every ability of the component but `except` in state 3 or more about `except` (vtable slot 0x138).
void CAbilityEnergyWave_NotifyOtherAbilitiesInStateAtLeast3(CAbilityComponent* self, CAbility* except);
UnkHitFlags_b152 fn_7100301c0c(void*, void*, void*);
extern void* g_unk_71726af0b0;  // What plays the sounds at a position.
extern UnkEmmyRegistry_b55* g_unk_71726af0a8;
extern void* g_unk_7101a22858[];
extern void* g_unk_7101a22878[];
UnkWeakTarget* fn_7100302014(void* owner);
UnkWeakTarget* fn_7100303054(void* owner);  // Looks the component up in the owner.
// Points the handle in `holder` at the weak handle of that component, unless it still has a target.
void CAbilityEnergyWave_RefreshOwnerComponentHandle(void* owner, UnkWeakHandle** holder);
// The ability's tunables (CAbilityEnergyWave::CTunableAbilityEnergyWave).
struct UnkTunableValue_ovb0262g;
extern UnkTunableValue_ovb0262g g_unk_71726af060;
void fn_71002fe5f8(CAbilityComponent* self, f32 dt);
void* fn_7100300bd0(void*, u32);
