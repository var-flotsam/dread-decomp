#pragma once

// UnkTU204: functions and globals of this translation unit that other files use (tools/tu_map.py unit 204,
// 0x710016998c-0x710016a37c). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkRegistry_710014c790;
struct UnkCountedResource;
namespace base::global {
class CFilePathStrId;
class CStrId;
}  // namespace base::global

// The shader binary's path for a name, "system/shd/<name>_nx.bshdat" (src/game/UnkShaderPath.cpp).
base::global::CFilePathStrId fn_7100169c74(const base::global::CStrId& name);

// The registry of src/base/spatial/UnkFrustumCull.cpp (UnkCullRegistry there): a chained hash set of pointers,
// buckets indexed by the key's low 32 bits modulo the capacity, and a list of all nodes.
extern void* g_unk_716e34c2e0;
void* fn_7100169be4(UnkRegistry_710014c790* registry, const void* key);
// Registers the resource under a name in the registry.
void fn_7100169d28(UnkRegistry_710014c790* registry, const base::global::CStrId& name, UnkCountedResource* resource);
