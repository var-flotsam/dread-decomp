#pragma once

#include "base/UnkEmptyDtorElem.h"
#include "types.h"

struct UnkElem {
    u32 field_0x0;

    // Copied bytewise: the original reloads the array pointer after the store.
    UnkElem(UnkElem&& other) { __builtin_memcpy(this, &other, sizeof(*this)); }
    ~UnkElem() { fn_71002874dc(this); }
};
