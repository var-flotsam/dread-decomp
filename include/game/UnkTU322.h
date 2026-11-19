#pragma once

// UnkTU322: functions of this translation unit that other files call (tools/tu_map.py unit 322,
// 0x710021c4cc-0x710021d8ac). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkRenderPool;
struct UnkCountedResource;
struct UnkModelInstance;
// The render pool's creator for model instances (src/base/spatial/UnkModelInstanceSet.cpp): an instance
// named `name` drawing `resource`, added to the pool and returned, also through `out` when given.
UnkModelInstance* fn_710021d3d4(UnkRenderPool* pool, UnkCountedResource* resource, UnkModelInstance** out,
                                const base::global::CStrId* name);
