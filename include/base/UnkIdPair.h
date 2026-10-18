#pragma once

// A value with a u32 at +8 and a string id at +0x10, copy-assigned member by member
// (fn_7100b96dec and its copies). What the first 8 bytes hold is unknown (not assigned).

#include "base/global/CStrId.h"
#include "types.h"

namespace base {

template <class Tag>
struct UnkIdPair {
    u8 unk_0x0[0x8];
    u32 field_0x8;
    global::CStrId id;

    UnkIdPair& operator=(const UnkIdPair& other) {
        if (this != &other) {
            field_0x8 = other.field_0x8;
            id = other.id;
        }
        return *this;
    }
};

}  // namespace base
