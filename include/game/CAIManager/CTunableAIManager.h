#pragma once

// CTunableAIManager: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2540,
// 0x7100da7404-0x7100daf1cc). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkManager_71726af468;
struct UnkReflType;
struct UnkTUData;
struct UnkTUHeader;
struct UnkAIManager_da92b0;
struct UnkAITarget_da92b0;
struct UnkEmmyRegistry_b55;
struct UnkGrabVec_b58a3;
struct UnkTimedRef;
void fn_7100daeb50(UnkEmmyRegistry_b55* registry, u32 kind, const UnkGrabVec_b58a3* pos, const UnkGrabVec_b58a3* dir);
void fn_7100da684c();
void fn_7100da6ab8();
void fn_7100da6e50(void*) __attribute__((visibility("hidden")));  // Destroys an array.
void fn_7100da6f50(void*) __attribute__((visibility("hidden")));  // Destroys an array.
void fn_7100da9194(UnkTimedRef* self);
void fn_7100da9bd4(UnkAIManager_da92b0* self, UnkAITarget_da92b0* object);
void fn_7100dab9bc() __attribute__((visibility("hidden")));
void fn_7100dab9c8() __attribute__((visibility("hidden")));
void fn_7100dab9d4() __attribute__((visibility("hidden")));
void fn_7100dab9e0() __attribute__((visibility("hidden")));
extern char g_unk_71727bc3a8;
extern char g_unk_71727bc508;
extern char g_unk_71727bc550;
bool fn_7100da9254(UnkManager_71726af468* self);
void fn_7100da928c(UnkManager_71726af468* manager);
extern UnkReflType g_unk_71727bc290;  // CAIManager::EAIGroup.
void fn_7100da929c(UnkManager_71726af468* self, f32 value);
extern UnkReflType g_unk_71727bc3f0;
extern UnkTUHeader g_unk_71727bc598 __attribute__((visibility("hidden")));
extern UnkTUData g_unk_71727bc5b0 __attribute__((visibility("hidden")));
