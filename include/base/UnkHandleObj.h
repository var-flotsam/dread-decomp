#pragma once

#include "types.h"

// A value and a reference-counted weak handle (released through vf_0x10; compared by target).
// The count is a union member (no type-based alias info: the handle is reloaded after the
// count changes).
struct UnkHandleObj {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        s32 ref_count;
    };
    void* target;
};
