#pragma once

#include "types.h"

struct UnkPathId;

struct UnkPathKey {
    u8 data[0x10];
    UnkPathKey(const UnkPathId* path);  // fn_710008d2ec
    ~UnkPathKey();                      // fn_710008d8e8
};
