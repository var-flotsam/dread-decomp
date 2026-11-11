#pragma once

// UnkTU204: functions and globals of this translation unit that other files use (tools/tu_map.py unit 204,
// 0x710016998c-0x710016a37c). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

// The registry of src/base/spatial/UnkFrustumCull.cpp (UnkCullRegistry there): a chained hash set of pointers,
// buckets indexed by the key's low 32 bits modulo the capacity, and a list of all nodes.
extern void* g_unk_716e34c2e0;
