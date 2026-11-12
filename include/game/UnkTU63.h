#pragma once

// UnkTU63: functions and globals of this translation unit that other files use (tools/tu_map.py unit 63,
// 0x71000872f4-0x7100088948). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkRegistry_0x70;

extern void* g_unk_7101a1b030[];
void fn_7100087dc8(UnkRegistry_0x70* self, void* entry, u32 kind);  // Calls the registry's handler.
void* fn_7100087740(UnkRegistry_0x70* self, const base::global::CStrId* name);  // The entry registered under `name`.
