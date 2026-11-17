#pragma once

// CSpittailMagmaBallMovementComponent: functions of this translation unit that other files call (tools/tu_map.py unit 1069,
// 0x71006c9fa0-0x71006cb374). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkSpittailMagmaBallMovement;
struct UnkModel;
struct UnkRenderPool;
void fn_71006cae2c(UnkRenderPool*, const base::global::CFilePathStrId&, UnkModel**, base::global::CStrId*);
void fn_71006ca5d4(UnkSpittailMagmaBallMovement* self, void* c);
