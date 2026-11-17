#pragma once

// The blend stack of unit 177 (game/UnkTU177.h): a base blend state, the state last applied, and up to 0x3f
// pushed states; the top one (the base when the stack is empty) is applied to NVN. Names and layouts are
// placeholders.
#include "types.h"

struct UnkBlendState {
    s32 field_0x0;
    s32 field_0x4;
    s32 field_0x8;
    s32 mode;  // 0xc
    s32 field_0x10;
    s32 field_0x14;
    bool enable;  // 0x18
    u8 field_0x19;  // Zero locks the state: it is not applied again.
};
static_assert(sizeof(UnkBlendState) == 0x1c, "");

struct UnkBlendStack {
    u8 applied;  // 0x0
    u8 pad_0x1[7];
    u32 count;  // 0x8
    u8 pad_0xc[0x24];
    UnkBlendState base;  // 0x30
    UnkBlendState current;  // 0x4c
    UnkBlendState entries[0x3f];  // 0x68
};
static_assert(__builtin_offsetof(UnkBlendStack, entries) == 0x68, "");

extern UnkBlendStack g_unk_716e29c6d0 __attribute__((visibility("hidden")));
