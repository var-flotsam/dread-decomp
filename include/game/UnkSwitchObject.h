#pragma once

// An object that is switched on or off, as fn_7100146e60 (src/game_lo/fn_7100146e60.cpp) sees it:
// a state value at +0x244, a busy flag at +0x240, and an owner at +0x160 whose member at +0x78
// receives the flag. The callers pass components found by hash or entries of a component list.
// Only the members the definition touches are spelled out; names and layout are placeholders.

#include "types.h"

struct UnkSwitchOwner {
    u8 pad_0x0[0x78];
    void* field_0x78;
};

struct UnkSwitchObject {
    u8 pad_0x0[0x28];
    u8 field_0x28[0xf9];
    bool field_0x121;
    u8 pad_0x122[0x3e];
    UnkSwitchOwner* field_0x160;
    char field_0x168[0xd8];
    bool field_0x240;
    u8 pad_0x241[3];
    s32 field_0x244;
};
static_assert(__builtin_offsetof(UnkSwitchObject, field_0x244) == 0x244, "UnkSwitchObject placeholder layout");

// Switches `self` on or off. Defined in src/game_lo/fn_7100146e60.cpp.
void fn_7100146e60(UnkSwitchObject* self, bool enable);
// Unit 133's switch of the same object (src/game_lo/fn_7100145e94.cpp).
void fn_7100145e94(UnkSwitchObject* self, bool enable);
