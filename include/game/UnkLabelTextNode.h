#pragma once

#include "types.h"

// The text node of a label's text object (UnkLabelTextObject::node, game/UnkLabelTextObject.h): the
// pointer CText's refresh reads at +0x80 and the colour digit ('0' to '9') a label run draws with at
// +0xe4. Names and layout are placeholders.
struct UnkLabelTextNode {
    u8 unk_0x0[0x80];
    void* field_0x80;
    u8 unk_0x88[0x5c];
    u32 color;
};
static_assert(__builtin_offsetof(UnkLabelTextNode, color) == 0xe4, "UnkLabelTextNode placeholder layout");
