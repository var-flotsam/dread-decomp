#pragma once

// UnkTU67: functions of this translation unit that other files call (tools/tu_map.py unit 67,
// 0x710008b6d4-0x710008c7d4). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "types.h"

struct UnkFilePathList;
bool fn_710008c3bc(void* base_object, const base::global::CFilePathStrId& path, UnkFilePathList* out);
