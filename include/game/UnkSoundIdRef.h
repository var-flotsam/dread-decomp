#pragma once

#include "game/UnkSoundIdInstance.h"
#include "types.h"

struct UnkSoundIdRef {
    UnkSoundIdInstance* ptr;

    UnkSoundIdRef(const UnkSoundIdRef& other) : ptr(other.ptr) {
        if (ptr)
            __atomic_fetch_add(&ptr->ref_count, 1, __ATOMIC_SEQ_CST);
    }
    ~UnkSoundIdRef() {
        if (UnkSoundIdInstance* p = ptr) {
            __atomic_fetch_sub(&p->ref_count, 1, __ATOMIC_SEQ_CST);
            if (__atomic_load_n(&p->ref_count, __ATOMIC_SEQ_CST) == 0)
                p->vf_0x10();
        }
    }
};
