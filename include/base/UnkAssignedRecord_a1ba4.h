#pragma once

#include "types.h"

// The reflected value's copy-assignment (declared as src/base/reflection/UnkReflResolve.cpp does, over a record of
// UnkReflValue's size).
// The reflected value's copy-assignment (the declaration is src/game_s1/fn_71000d6310.cpp's, over a
// record of UnkReflValue's size).
// The reflected value's copy-assignment (declared as src/game_s1/fn_71000d6310.cpp does, over a
// record of UnkReflValue's size).
struct UnkAssignedRecord_a1ba4 {
    u64 unk_0x0[0x18 / 8];
    UnkAssignedRecord_a1ba4& operator=(const UnkAssignedRecord_a1ba4& other);
};
