#pragma once

// UnkTU67: functions of this translation unit that other files call (tools/tu_map.py unit 67,
// 0x710008b6d4-0x710008c7d4). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkFilePathList;
struct UnkFilePathList_71000839a0;
// Lists the files under `path` into `out`.
bool fn_710008c3bc(void* base_object, const base::global::CFilePathStrId& path, UnkFilePathList_71000839a0* out);
u32* fn_710008c504(base::global::CRntVector<u32>* self, u32 index);  // The set's index array element accessor, out of line.
