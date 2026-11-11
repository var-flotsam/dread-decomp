#pragma once

// UnkTU39: functions and globals of this translation unit that other files use (tools/tu_map.py unit 39,
// 0x710002f4e4-0x7100065d24). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkFloatList_b46a1;
struct UnkMoveSlot_0xb0;
struct UnkV4_b47;
bool fn_710002fb70(void* timer);
void fn_710002f67c(void*);
void fn_710002fa90(void*);
void fn_710002fb80(void*, f32);
UnkV4_b47 fn_7100030770(const UnkV4_b47* a, const UnkV4_b47* b);
UnkV4_b47 fn_7100030830(const UnkV4_b47* a, f32 s);
void fn_710002f588(void* self, const void* other);  // Copy constructor.
void* fn_710002f90c(void* dst, const void* src);
void fn_7100030128(char* self, bool flag, void* arg);
void fn_7100030a50(UnkMoveSlot_0xb0*);
void fn_7100030ce0(void*);
f32 fn_7100030dec(UnkFloatList_b46a1* list);
void fn_710003152c(void* self, const f32* value);
extern void* g_unk_7101a1a090[];
extern void* g_unk_7101a1a0b0[];
