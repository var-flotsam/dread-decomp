#pragma once

// UnkTU2704: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2704,
// 0x7101120afc-0x71011336b0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkStruct_7172a0768c;
struct UnkObj_7101129240;
void fn_71011258e4(void* self);
void fn_7101129b08(UnkObj_7101129240* self, bool flag);
void fn_7101125c90(char*);
void fn_7101126af4(void* self);
void fn_710112e7e0(void* self);  // Re-points `current` at (index, element).
extern UnkStruct_7172a0768c g_unk_7172a0768c;  // The id list's count, its ids after it.
const base::global::CStrId* fn_710112bf20(void* link);
