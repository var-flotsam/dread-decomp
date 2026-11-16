#pragma once

// UnkTU107: functions of this translation unit that other files call (tools/tu_map.py unit 107,
// 0x710010d604-0x7100110c54). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "types.h"

struct UnkNavMeshTree;
struct UnkBoxVector_b0236;
struct UnkVec3f;

void fn_710010d9f4(void* p);
bool fn_710010ddc4(UnkNavMeshTree* self, base::global::CRntFile* file);
// Sets the number of boxes: grows the array if needed and constructs the new ones.
void fn_710010e364(UnkBoxVector_b0236* self, u32 n);
// The mean of the centres of boxes [first, last) (z is 0).
UnkVec3f fn_710010df80(void* unused, UnkBoxVector_b0236* boxes, u32 first, u32 last);
// The mean squared distance per axis from `point` to the centres of boxes [begin, end).
UnkVec3f fn_710010e15c(const void* self, UnkBoxVector_b0236* boxes, u32 begin, u32 end, const UnkVec3f* point);
