#pragma once

// UnkTU2563: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2563,
// 0x7100e31c18-0x7100e65204). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkArraysRecord;
struct UnkPolyValues;
struct UnkPolyVector;
struct UnkRecord60;
struct UnkRecord60Array;
void fn_7100e4a1d0(UnkRecord60* record);  // Destructor.
UnkArraysRecord* fn_7100e4f444(UnkArraysRecord* self, const UnkArraysRecord* other);  // operator=.
UnkPolyVector* fn_7100e4e898(UnkPolyVector* self, const UnkPolyVector* other);
void fn_7100e4e310(char* self);
void fn_7100e4fd1c(UnkRecord60* self, const UnkRecord60* other);
void fn_7100e51078(void* dst, void* src);
void fn_7100e54ad8(UnkRecord60Array*, u32);
void fn_7100e4ea08(UnkPolyVector* self, const UnkPolyValues* value);  // PushBack.
void fn_7100e56814(void*, u32);
extern void* g_unk_7101aa9d38[];
