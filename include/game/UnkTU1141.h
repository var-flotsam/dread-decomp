#pragma once

// UnkTU1141: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1141,
// 0x71007142e8-0x71007153d4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkFlags_71007142e8;
struct UnkMotionRule;
struct UnkStruct_71007142e8;
struct UnkTUHeader;
struct UnkWeighted_7100714808;
void fn_71007143c8(UnkStruct_71007142e8* self, UnkFlags_71007142e8 flags);
void fn_7100714604(UnkWeighted_7100714808* self, const UnkWeighted_7100714808* other);  // Assigns.
void fn_71007149f4(UnkWeighted_7100714808* self, const UnkWeighted_7100714808* other, f32 weight, bool flag);
void fn_7100714eb4(void* part, const u32* flags);
void fn_71007142e8(UnkStruct_71007142e8* self);
// The unit's header data (game/UnkTUInit.h; fn_7100714700 reads its first word as motion-part bits) and its
// nine motion-part rules (game/UnkMotionRule.h), all built by its initialiser, fn_7100714ec0.
extern UnkTUHeader g_unk_717270ec80 __attribute__((visibility("hidden")));
extern UnkMotionRule g_unk_717270ebc8;
extern UnkMotionRule g_unk_717270ebdc;
extern UnkMotionRule g_unk_717270ebf0;
extern UnkMotionRule g_unk_717270ec04;
extern UnkMotionRule g_unk_717270ec18;
extern UnkMotionRule g_unk_717270ec2c;
extern UnkMotionRule g_unk_717270ec40;
extern UnkMotionRule g_unk_717270ec54;
extern UnkMotionRule g_unk_717270ec68;
