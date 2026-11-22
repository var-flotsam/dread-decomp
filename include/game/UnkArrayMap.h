#pragma once

#include "types.h"

struct UnkArrayMapNode;

struct UnkArrayMap {
    u8 unk_0x0[0x10];
    UnkArrayMapNode** buckets;
    u32 bucket_count;
    u32 modulus;
};
