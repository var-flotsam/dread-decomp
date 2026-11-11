#pragma once

// UnkTU99: functions of this translation unit that other files call (tools/tu_map.py unit 99,
// 0x71000f43c0-0x71001043e4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkTunableTokens;
void fn_71000fbe1c(void* self, u32 a, f32 b, f32 c);
void fn_71000fc570(const char* text, const char* separator, UnkTunableTokens* out);
void fn_71000fc7cc(UnkTunableTokens* self);
