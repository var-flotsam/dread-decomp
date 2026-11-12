#pragma once

// CPhotoModeCameraCtrl: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1490,
// 0x71009ca6f0-0x71009cbfb8). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkFreeCameraCallback_b0058;

void fn_71009ca9fc(void*, const char*, u32);
extern void* g_unk_7101a89018[];
UnkFreeCameraCallback_b0058** fn_71009cb3e0(base::UnkVector<UnkFreeCameraCallback_b0058*>* self, u32 index);
