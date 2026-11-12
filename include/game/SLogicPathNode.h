#pragma once

// SLogicPathNode: functions of this translation unit that other files call (tools/tu_map.py unit 2657,
// 0x7101053e24-0x7101061fa0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

struct UnkMinimapPath;
struct UnkArgObj;
struct UnkDashPath;
struct UnkInsertTarget;
struct UnkMinimapPathPoint;
UnkMinimapPathPoint fn_71010596ac(void* path, u32 index, f32 t);
f32 fn_7101059514(UnkDashPath* path);
void fn_7101059794(f32 distance, UnkDashPath* path, u32* index, f32* fraction);
void fn_710106146c(UnkInsertTarget* target, const UnkArgObj* obj, u32 value);
void fn_71010592b0(UnkMinimapPath* path, const base::math::CVector3D* point);  // Adds a point.
