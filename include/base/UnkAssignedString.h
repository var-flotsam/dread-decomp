#pragma once

#include "types.h"

// A value and a string (base::global::CRntString's layout, assigned by fn_710007eaa0).
struct UnkAssignedString {
    u64 unk_0x0[6];

    UnkAssignedString& operator=(const UnkAssignedString& other);
};
