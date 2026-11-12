#pragma once

// UnkTU0: functions and globals of this translation unit that other files use (tools/tu_map.py unit 0,
// 0x7100000030-0x710000147c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkGfxFrameSource;
struct UnkEngineObject_0xf0;
struct UnkGameObject_0x150;
struct UnkGameObject_0x60;
struct UnkLanguage;
struct UnkLoadingScreen;
struct UnkSoundHolder;
struct UnkStatsObject;
struct UnkStruct_0x128;
struct UnkDropTarget;
struct UnkUserDataOwner;

bool fn_7100000790(const char* name, const char* fmt, ...);  // Calls a script function.
void fn_71000003d4(void*, const char*, bool);  // The string-id constructor (base/global/CStrId.h).
extern UnkUserDataOwner* g_unk_7101d47308;
extern UnkDropTarget* g_unk_7101d47318;
extern UnkSoundHolder* g_unk_7101d47340;  // The sound holder singleton (0x48 bytes); its first word is a sound group.
extern UnkStruct_0x128* g_unk_7101d47320;  // The scene-state singleton (0x150 bytes, created on first use).
// The language singleton (0x2068 bytes, created on first use): it gives the localized texts.
extern UnkLanguage* g_unk_7101d47358;
extern UnkStatsObject* g_unk_7101d47100;  // The statistics object (0x78 bytes) the device's memory size is reported to.
extern UnkLoadingScreen* g_unk_7101d47380;
extern UnkEngineObject_0xf0* g_unk_7101d47338;  // An object of unit 52 (0xf0 bytes).
extern UnkGameObject_0x150* g_unk_7101d47370;
extern UnkGameObject_0x60* g_unk_7101d47378;
extern void* g_unk_7101d47330;
extern void* g_unk_7101d47368;
extern UnkGfxFrameSource* g_unk_7101d47328;
