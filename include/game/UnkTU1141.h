#pragma once

// UnkTU1141: functions of this translation unit that other files call (tools/tu_map.py unit 1141,
// 0x71007142e8-0x71007153d4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkFlags_71007142e8;
struct UnkStruct_71007142e8;
struct UnkWeighted_7100714808;
void fn_71007143c8(UnkStruct_71007142e8* self, UnkFlags_71007142e8 flags);
void fn_7100714604(UnkWeighted_7100714808* self, const UnkWeighted_7100714808* other);  // Assigns.
void fn_71007149f4(UnkWeighted_7100714808* self, const UnkWeighted_7100714808* other, f32 weight, bool flag);
void fn_7100714eb4(void* part, const u32* flags);
