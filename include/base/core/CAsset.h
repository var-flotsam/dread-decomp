#pragma once

// CAsset: functions of this translation unit that other files call (tools/tu_map.py unit 24,
// 0x7100016890-0x7100018a44). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkAssetResRef;
struct UnkRecord80;
struct UnkStrIdPathPair;
void fn_7100016f88(void* self);
void fn_7100016ad8(void* self);
void CAsset_CopyConstructRecord(UnkRecord80* self, const UnkRecord80* other);
void fn_7100017ce8(UnkRecord80* record);  // The record's destructor.

// The resource the ids name (fn_710002038c in the resource registry) when both are set; an empty
// reference otherwise. `self` is not read.
UnkAssetResRef CAsset_GetResourceForIdAndPath(void* self, const UnkStrIdPathPair* ids);
