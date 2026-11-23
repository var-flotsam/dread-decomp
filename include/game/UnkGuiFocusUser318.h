#pragma once

#include "game_s2/UnkWeakHandle.h"

// Unless the flag at 0x181 is set, refers to the focused object at 0x318 (or to nothing), passes it
// 0 and this object 1, then hands this object to the root.
struct UnkGuiFocusUser318 {
    u8 unk_0x0[0x181];
    bool field_0x181;
    u8 unk_0x182[0x196];
    UnkWeakHandle* field_0x318;
};
