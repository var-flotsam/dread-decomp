#pragma once

// UnkTU25: functions and globals of this translation unit that other files use (tools/tu_map.py unit 25,
// 0x7100018a44-0x710001f9ac). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "types.h"

struct UnkStrIdPathPairVec_b45a1;
struct UnkInlineString;
struct UnkPathIds;
struct UnkPathPairTemp_b45a1;
void fn_710001f074(UnkPathIds* out, const UnkInlineString* path);
UnkPathPairTemp_b45a1 fn_710001ab24(void* path);
void fn_710001e4e8(UnkStrIdPathPairVec_b45a1* self, u32 capacity);
extern base::reflection::CType g_unk_7101d4dad8;
