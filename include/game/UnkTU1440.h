#pragma once

// UnkTU1440: functions of this translation unit that other files call (tools/tu_map.py unit 1440,
// 0x7100969000-0x710096939c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkUnit1444Base;
struct UnkUnit1441Apply_A4_b74;
struct UnkUnit1441Msg_A4_b74;
struct UnkUnit1444_ovb0177;
void fn_7100969000(char* self, u32 arg);
void fn_71009690a0(UnkUnit1441Apply_A4_b74* self, const UnkUnit1441Msg_A4_b74* msg, bool reset);
void fn_7100969148(UnkUnit1444_ovb0177* self);
void fn_7100969058(UnkUnit1444Base* self);
