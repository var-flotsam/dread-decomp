#pragma once

// CPolygonCollectionShape: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2515,
// 0x7100d625fc-0x7100d6736c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

struct UnkTileOutline;
struct UnkTileOutlines;
void fn_7100d62bd8(void* p);
void fn_7100d62b04(void*);  // The POLYCOLLECTION2D shape's constructor.
void fn_7100d62d40(UnkTileOutlines* self, UnkTileOutline* outline);
void fn_7100d62b68(char*, const char*);
void fn_7100d633ec(UnkTileOutlines* self);
extern char g_unk_7101aa8b80[];  // The shape's vtable.
bool fn_7100d65fb0(void* area, const base::math::CVector2D* point);
