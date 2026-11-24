#pragma once

// CShelmitAIComponent: functions of this translation unit that other files call (tools/tu_map.py unit 1054,
// 0x71006b95f0-0x71006bcf84). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkShelmitAI;
struct UnkShelmitAI_b38;
struct UnkShelmitAI_b49a3;
struct UnkShelmitAI_sis;
void fn_71006ba194(UnkShelmitAI* self);
void fn_71006ba4fc(void* self);
void fn_71006baa5c(UnkShelmitAI_b49a3* self, u32 a, u32 b);
void fn_71006badf4(UnkShelmitAI* self, f32 value);
s32 fn_71006bc178(UnkShelmitAI_sis*);
// The two model flags the mode sets and clears (fn_7101172764, fn_7101172780): read-only data.
extern char g_unk_710164a730[];
extern char g_unk_710164a738[];
void fn_71006baf90(UnkShelmitAI_b38* ai);
