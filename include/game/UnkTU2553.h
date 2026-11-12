#pragma once

// UnkTU2553: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2553,
// 0x7100de277c-0x7100e04d68). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkAbility;
struct UnkEnergyWavePlayer2_ovb22;
struct UnkGameManager;
struct UnkOwner;
struct UnkProfileSaveArg;
struct UnkRemovedValue_b60;
struct UnkSpawnGroupKey;
struct UnkVec;
struct UnkWeakTarget;
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
UnkPlayer* fn_7100dec788(UnkGameManager*);
UnkOwner* fn_7100defcd4(UnkGameManager*);
UnkWeakTarget* fn_7100debbc8(UnkGameManager* manager, const u64* key, const u32* kind);
void* fn_7100df55e4(UnkGameManager*);
void fn_7100e038b0(UnkGameManager* dispatcher, void* key, UnkVec* vec, u64 arg);
void* fn_7100df5460(UnkGameManager* manager);
void fn_7100df6c78(UnkGameManager* self, f32 a, f32 b);
bool fn_7100dfbe3c(UnkGameManager*);
void fn_7100e03a78(UnkGameManager*, void*);
void fn_7100de9730(UnkGameManager* manager, UnkSpawnGroupKey* key, void* object);
void fn_7100debc28(UnkGameManager*, UnkRemovedValue_b60*);
void fn_7100df6d3c(UnkGameManager* self, void* settings, f32 a, f32 b);
s32 fn_7100df99f8(UnkGameManager* self, u32 a, const UnkProfileSaveArg* arg, u32 b);
void fn_7100de6ee8(UnkGameManager* self);
bool fn_7100decc5c(UnkGameManager* self);
bool fn_7100df5a4c(UnkGameManager* self);
bool fn_7100ded930(UnkGameManager* self);
void fn_7100dee46c(UnkGameManager* self);
void* fn_7100df5440(UnkGameManager* manager);
UnkAbility* fn_7100df554c(UnkGameManager* manager);
bool fn_7100df5630(UnkGameManager* manager);
s32 fn_7100dffc58(UnkGameManager* manager);
bool fn_7100deabc4(UnkGameManager* manager);
void fn_7100decc74(UnkGameManager* self);
bool fn_7100df5740(UnkGameManager*);
void* fn_7100df57f4(UnkGameManager* manager);
bool fn_7100df5c20(UnkGameManager* manager, UnkEnergyWavePlayer2_ovb22* player);
void fn_7100df5cec(UnkGameManager* manager, void* target);
bool fn_7100dfbe78(UnkGameManager* manager, void* record);
void fn_7100dffc3c(UnkGameManager* self, f32 v);
void fn_7100dffd48(UnkGameManager* manager);
