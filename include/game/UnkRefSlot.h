#pragma once

#include "game_s2/UnkWeakHandle.h"

// A counted reference whose slot is released in place.
struct UnkRefSlot {
    UnkRefObject* ptr;

    void Release() {
        if (UnkRefObject* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};
