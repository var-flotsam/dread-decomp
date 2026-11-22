#pragma once

#include "types.h"

// fn_7100af8450: the gallery's event handler. After the container's own (fn_71011d99e4), an event of kind 2
// (+0x20) opens the gallery: the archives inspector (cached at +0x2a8) is told whether +0x255 is clear
// (fn_7100af2d7c), the selection goes back to 0 with every entry deselected, then either the next queued entry
// is revealed (registering the gallery's key) or the first one is selected.
struct UnkGuiEvent_b49 {
    u8 pad_0x0[0x20];
    u64 kind;  // 0x20
};
