#pragma once

#include "base/global/CFilePathStrId.h"
#include "types.h"

// A file-path id built from a string; returned in memory.
struct UnkPathId_lfa6ad0 {
    base::global::UnkFilePathStrIdBase id;
    explicit UnkPathId_lfa6ad0(const base::global::CRntString& str) { fn_710001a270(&id, str, true); }
    UnkPathId_lfa6ad0(const UnkPathId_lfa6ad0& other);
};
