#pragma once

// UnkTU56: functions of this translation unit that other files call (tools/tu_map.py unit 56,
// 0x710007bb30-0x710007e6c4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CRntFile.h"
#include "types.h"
s64 fn_710007cf30(void* stream, const char* format, ...);
s64 fn_710007cee4(void*, const char**);  // The next string of the stream, in place.
// Copy-constructs a file from another one (its contents copied into a block of its own).
void fn_710007beac(base::global::CRntFile* self, const base::global::CRntFile* other, u32 flags, base::UnkAllocator* allocator);
// The file's constructor over existing data.
void fn_710007c080(base::global::CRntFile* file, void* data, u32 flags, u64 size);
// Formatted stream write ("i" integer, "b" bool, "f" float, "s" string).
s64 fn_710007cb4c(void* stream, const char* format, ...);
