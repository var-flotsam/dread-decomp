#pragma once

// CAABoxShape2D: functions of this translation unit that other files call (tools/tu_map.py unit 2511,
// 0x7100d5d23c-0x7100d5f00c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkBoxShape_d52bd0;
struct UnkPoint2D_d52bd0;
struct UnkCollisionShape;
void fn_7100d5e1b4(void* shape);  // The AABOX2D shape's constructor.
void fn_7100d5e2c8(UnkCollisionShape* self);
struct UnkBox2D;
void fn_7100d5e1f8(UnkCollisionShape* self, const UnkBox2D* box);  // Sets the box.
UnkPoint2D_d52bd0 fn_7100d5e540(const UnkBoxShape_d52bd0* box);
