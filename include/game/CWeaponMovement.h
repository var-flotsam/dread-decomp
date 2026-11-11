#pragma once

// CWeaponMovement: functions of this translation unit that other files call (tools/tu_map.py unit 1624,
// 0x7100ac6c44-0x7100ac88dc). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkComponent_0x5b8;
struct UnkSplash_b59a4;
void fn_7100ac7224(UnkComponent_0x5b8* self);  // Derived classes call it as their base's constructor.
s32 fn_7100ac82c0(UnkSplash_b59a4* self, void* other);  // Returns 1 when the hit was taken.
