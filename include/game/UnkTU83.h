#pragma once

// UnkTU83: functions of this translation unit that other files call (tools/tu_map.py unit 83,
// 0x71000a4598-0x71000e5f9c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkPart48;
struct UnkStrVector;
struct UnkStruct_0x28;
struct UnkVec3_cs;
struct UnkVersionEntry;
struct UnkVersionRegistry;
UnkVersionEntry* fn_71000a5118(UnkVersionRegistry* self, const u64* id);
void fn_71000ce19c(UnkStruct_0x28* self, const UnkStruct_0x28* other);
void fn_71000cbae4(UnkPart48* self_, const UnkPart48* other_);
UnkVec3_cs* fn_71000cc374(void* array, u32 index);
void fn_71000e0524(UnkStrVector* self, UnkStrVector* other);
void fn_71000bce50(void*);
void fn_71000be6c0(void*);
