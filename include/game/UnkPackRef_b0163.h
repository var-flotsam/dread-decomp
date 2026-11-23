#pragma once

#include "game/UnkPackRefTarget_b0163.h"

struct UnkPackRef_b0163 {
    UnkPackRefTarget_b0163* ptr;
    ~UnkPackRef_b0163() {
        if (UnkPackRefTarget_b0163* p = ptr) {
            __atomic_fetch_sub(&p->ref_count, 1, __ATOMIC_SEQ_CST);
            if (__atomic_load_n(&p->ref_count, __ATOMIC_SEQ_CST) == 0)
                reinterpret_cast<void (*)(UnkPackRefTarget_b0163*)>(p->vtable[0x10 / 8])(p);
        }
    }
};
