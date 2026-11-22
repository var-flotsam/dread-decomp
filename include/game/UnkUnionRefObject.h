#pragma once

#include "types.h"

// A reference whose object's count is a union member (no type-based alias info: the count of
// the array is reloaded after each release).
struct UnkUnionRefObject {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        s32 ref_count;
    };
};
