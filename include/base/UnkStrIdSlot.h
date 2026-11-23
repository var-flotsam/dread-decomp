#pragma once

#include "base/global/CStrId.h"
#include "types.h"

// Room for a string id that the out-of-line constructor (fn_7100000850) builds; it is left
// without a destructor, since its string is always handed on.
// Here each id built in it is a temporary: handed on, then destroyed by hand.
union UnkStrIdSlot {
    base::global::CStrId id;
    UnkStrIdSlot() {}
    ~UnkStrIdSlot() {}
};
