#pragma once

#include "types.h"

struct UnkPathLink {
    u32 field_0x0;
    u32 field_0x4;

    // Zeroed and copied word by word, untyped: the original reloads the array after each store.
    UnkPathLink() {
        __builtin_memset(&field_0x0, 0, sizeof(u32));
        __builtin_memset(&field_0x4, 0, sizeof(u32));
    }
    UnkPathLink(UnkPathLink&& other) {
        field_0x0 = other.field_0x0;
        field_0x4 = other.field_0x4;
    }
};
