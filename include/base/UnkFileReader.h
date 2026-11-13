#pragma once

// UnkFileReader: one definition of a type that several sources had each written out with the same
// text (tools/struct_migrate.py). Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "types.h"

struct UnkFileReader {
    base::global::CRntFile file;
    u32 field_0x38;
    u32 field_0x3c;
};
