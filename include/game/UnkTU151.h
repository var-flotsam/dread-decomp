#pragma once

// UnkTU151: functions of this translation unit that other files call (tools/tu_map.py unit 151,
// 0x710014c660-0x710014dc2c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"
void fn_710014d9e8(void* block, u32 arg);

struct UnkSetupSub_b0237 {
    u8 pad_0x0[0x1];
};

struct UnkSetupArg_b0237 {
    u64 a;
    u32 b;
    u32 c;
};
void fn_710014d150(UnkSetupSub_b0237* sub, void* block, u32 flag);
void fn_710014d17c(UnkSetupSub_b0237* sub, UnkSetupArg_b0237* arg);
void fn_710014cb98(UnkSetupSub_b0237* sub, u32 a, void* id, u32 b, u32 c);
void fn_710014c864(UnkSetupSub_b0237* sub, void* p, u32 a);
u64 fn_710014d1a0(UnkSetupSub_b0237* sub);
