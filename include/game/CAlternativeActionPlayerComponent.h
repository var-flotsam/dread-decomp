#pragma once

// CAlternativeActionPlayerComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1138,
// 0x7100711600-0x7100712b80). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkAlternativeActionPlayer;
// Passed by value in one register: a word and a flag.
struct UnkSnapArg_b69a4 {
    u32 field_0x0;
    bool skip;
};
u32 fn_71007124d8(UnkAlternativeActionPlayer* self, UnkSnapArg_b69a4 arg);  // returns 0
bool fn_7100712538(UnkAlternativeActionPlayer* self, void*);
extern void* g_unk_7101a6c0b8[];
