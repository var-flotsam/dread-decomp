#pragma once

// UnkTU883: functions and globals of this translation unit that other files use (tools/tu_map.py unit 883,
// 0x71005ba094-0x71005c49d0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkCallback_ac3184;
struct UnkZiplineRailPart_b49;
void fn_71005c41bc(void*, UnkCallback_ac3184*);  // Registers.
UnkZiplineRailPart_b49* fn_71005c3084(void* object);
void fn_71005c4828(void*, void**);  // Unregisters.
extern base::global::CStrId g_unk_71726eb440 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_71726eb450 __attribute__((visibility("hidden")));
char* fn_71005c1008(u64 id);
