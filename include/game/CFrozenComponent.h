#pragma once

// CFrozenComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1260,
// 0x71007f8310-0x71007fbc58). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct CFrozenComponent2;
struct UnkFrostbiteComponent;
struct UnkFrostbiteRequest;
bool fn_71007f9f20(void* component);
bool fn_71007f9fd0(UnkFrostbiteComponent* self);
void fn_71007f9428(UnkFrostbiteComponent* self, u32 state);
bool fn_71007f9fb0(UnkFrostbiteComponent* self);
bool fn_71007fb280(UnkFrostbiteComponent* self, UnkFrostbiteRequest* request);
void fn_71007f8bc0(void*);
bool fn_71007f900c(void* self, void* arg);
void fn_71007f95b8(CFrozenComponent2* self, f32 value);
void fn_71007fa130(UnkFrostbiteComponent* self);
extern char g_unk_7172721120[] __attribute__((visibility("hidden")));
extern void* g_unk_7101a743c8[];  // The frozen state machine's vtable.
