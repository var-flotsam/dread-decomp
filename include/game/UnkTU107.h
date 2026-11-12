#pragma once

// UnkTU107: functions of this translation unit that other files call (tools/tu_map.py unit 107,
// 0x710010d604-0x7100110c54). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "types.h"

struct UnkNavMeshTree;

void fn_710010d9f4(void* p);
bool fn_710010ddc4(UnkNavMeshTree* self, base::global::CRntFile* file);
