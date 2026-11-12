#pragma once

// CPackManagerDevice: functions of this translation unit that other files call (tools/tu_map.py unit 66,
// 0x7100089284-0x710008b6d4). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/file/UnkFileDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/CDeviceQueue.h"
#include "types.h"

struct UnkFileRequestArgs;
// Unloads the file `path`.
void fn_710008987c(base::file::UnkFileDevice* self, const base::global::CFilePathStrId* path, u32 flags);
void fn_7100089754(base::file::UnkFileDevice* self, const base::global::CFilePathStrId* path, u32 a, u32 b, const UnkFileRequestArgs* args);
void fn_71000898c4(base::file::UnkFileDevice* self);
bool fn_710008aa90(base::file::UnkDevice* packs, const base::global::CFilePathStrId& path);
