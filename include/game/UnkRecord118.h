#pragma once

#include "types.h"

// Move-assignment of an array with a second pointer (destroying the elements through
// fn_7100ea7034, keeping the storage).
struct UnkRecord118 {
    u64 unk_0x0[0x23];

    ~UnkRecord118();
};
