#pragma once

// The unit 36 worker object (tools/tu_map.py unit 36, the NFP worker), the static g_unk_7101d4fc30:
// built in place by fn_710002cbec, its base part by fn_710002cff8, which starts the worker thread.
// Names and layout are placeholders; the offsets are the ones fn_710002cff8 writes.
#include <nn/os.h>

#include "types.h"

struct UnkUnit36Static {
    char* vtable;  // +0x0
    bool field_0x8;
    bool field_0x9;
    u32 field_0xc;
    u8 pad_0x10[0x440];
    u64 field_0x450;
    u8 pad_0x458[8];
    u64 field_0x460;
    nn::os::ThreadType thread;  // +0x468
    nn::os::EventType event;    // +0x628
};
