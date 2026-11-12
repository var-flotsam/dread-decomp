#pragma once

// UnkTU2640: functions of this translation unit that other files call (tools/tu_map.py unit 2640,
// 0x7100fe119c-0x7101007b10). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkPartFfca00;

// Out-of-line destructor of a music preset map's values (src/game/sound/UnkMusicPresets.cpp).
void fn_7100fee034(void*);

void fn_7100ffaa88(void*);
void fn_7100fefde0(void*);
UnkPartFfca00* fn_7100ffbad8(UnkPartFfca00* self, const UnkPartFfca00* other);
void fn_7101005d8c(void*);
