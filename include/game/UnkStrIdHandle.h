#pragma once

#include "base/global/CStrId.h"
#include "types.h"

struct UnkStrIdHandle {
    base::global::CStrId id;
    void* field_0x8;
    UnkStrIdHandle(const base::global::CStrId& id);  // fn_7100ae986c
};
