#pragma once

// UnkTU170: functions and globals of this translation unit that other files use (tools/tu_map.py unit 170,
// 0x710015334c-0x7100153c20). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkHeap_71001541a0;
struct UnkObject_0x130;
struct UnkPoolCount;
struct UnkPtrHolder;
struct UnkPool;
void fn_710015385c(UnkPool* pool, u64 size, s32 flags, void* device);
void fn_7100153968(UnkPool* pool, u64 size, u64 align);
extern UnkPtrHolder g_unk_716e275190;
extern UnkPoolCount g_unk_716e275794 __attribute__((visibility("hidden")));
extern UnkObject_0x130 g_unk_716e2751a0;
extern UnkObject_0x130 g_unk_716e2752d0;
extern UnkObject_0x130 g_unk_716e275400;
extern UnkObject_0x130 g_unk_716e275530;
extern UnkObject_0x130 g_unk_716e275660;
void fn_7100153a88(UnkHeap_71001541a0* pool, void* ptr);  // Frees through the heap.
