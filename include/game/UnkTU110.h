#pragma once

// UnkTU110: functions of this translation unit that other files call (tools/tu_map.py unit 110,
// 0x71001259f4-0x7100132c04). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSweep2D;
struct UnkSegment2D;
struct UnkContact2D;
bool fn_710012bcd0(const UnkSweep2D* sweep, const UnkSegment2D* seg, UnkContact2D* out);
struct UnkQuadCorners;  // two corner points, three u32 each
struct UnkQuadVertex;
template <typename T>
struct UnkQuadArray;  // a growable array (data, size, capacity, step, tag, allocator)
// Appends the quad's four corners to `vertices` and its two triangles to `indices` (winding by `flip`).
void fn_710012f880(const UnkQuadCorners* corners, UnkQuadArray<UnkQuadVertex>* vertices, UnkQuadArray<u16>* indices,
                   bool flip);
