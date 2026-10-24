#pragma once

// A large global object (reached through the GOT) whose flag at +0x318367c blocks the requests
// fn_71001c2764 and fn_71001c28f8 forward. Only that flag is known.

#include "types.h"

struct UnkGlobal_710b372a98 {
    u8 pad_0x0[0x318367c];
    bool field_0x318367c;
};
extern UnkGlobal_710b372a98* g_unk_710b372a98;
