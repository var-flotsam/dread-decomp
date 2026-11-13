#pragma once

// UnkEnumDesc and UnkEnumInfo: one definition of two types that several sources had each written out with the same
// text (tools/struct_migrate.py). Names and layouts are placeholders.
#include "types.h"

struct UnkEnumDesc {
    u8 pad_0x0[0x30];
    void* field_0x30;
};

struct UnkEnumInfo {
    u8 pad_0x0[8];
    UnkEnumDesc* desc_0x8;
};
