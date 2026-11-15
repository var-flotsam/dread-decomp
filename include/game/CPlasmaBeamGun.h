#pragma once

// CPlasmaBeamGun: functions of this translation unit that other files call (tools/tu_map.py unit 1311,
// 0x7100850d30-0x7100852924). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkFxPooledObject;
struct UnkFxCache;
// Registers the effect at `path` with the cache.
void fn_7100851bc0(UnkFxCache* cache, const base::global::CFilePathStrId* path, u32 kind, bool flag);
void fn_7100851d6c(UnkFxCache* cache, const base::global::CFilePathStrId* path, u32 kind, bool flag);
// The element at `index`, growing the array to reach it (base::global::CRntVector's GrowAt, out of line).
UnkFxPooledObject** fn_7100852548(base::global::CRntVector<UnkFxPooledObject*>* array, u32 index);
