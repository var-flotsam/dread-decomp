#pragma once

#include "game_s2/UnkWeakHandle.h"

// A counted reference whose destructor keeps the pointer in a register (UnkRefPtr reloads it).
struct UnkRefSlot_b49a3 {
    UnkRefObject* ptr;
    ~UnkRefSlot_b49a3() {
        if (UnkRefObject* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};
