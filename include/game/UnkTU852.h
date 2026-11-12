#pragma once

// UnkTU852: functions and globals of this translation unit that other files use (tools/tu_map.py unit 852,
// 0x710059c5d8-0x710059d9dc). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkHandleList;
struct UnkRefPtr;

void fn_710059c7b8(void* self);
void fn_710059c644(u8* container, u32 a, u32 b, u32 c, u64 d);
extern void* g_unk_7101a4d918[];
extern void* g_unk_7101a4d8f8[];
void fn_710059cecc(UnkHandleList* list, const UnkRefPtr& ref);
