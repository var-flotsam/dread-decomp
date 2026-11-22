#pragma once

#include "game_s2/UnkWeakHandle.h"

// As fn_7100b021fc with the reference at 0x238, which is only changed when there is a focused
// object.
struct UnkGuiFocusUser238 {
    u8 unk_0x0[0x238];
    UnkWeakHandle* field_0x238;
};
