#pragma once

// UnkTU604: functions and globals of this translation unit that other files use (tools/tu_map.py unit 604,
// 0x71003f8b30-0x71003faf9c). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"
extern void* g_unk_7101a32680[];  // The node list's vtable.
extern void* g_unk_7101a32660[];  // The node list's derived part's vtable.
void fn_71003fa9fc(void* list, u32 block, u32 flags, u32 alloc_flags, base::UnkAllocator* allocator);
char* fn_71003f92ac(void* value);
