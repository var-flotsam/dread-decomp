#pragma once

#include "game/UnkCopiedVector.h"

struct UnkBlockVector : UnkCopiedVector<u16> {
    __attribute__((always_inline)) UnkBlockVector& operator=(const UnkBlockVector& other) {
        if (this != &other) {
            count = 0;
            u32 reserve = other.capacity;
            if (capacity < reserve) {
                u16* p = static_cast<u16*>(allocator->vf_0x40((u64)reserve * sizeof(u16), 8, flags));
                if (data) {
                    __builtin_memcpy(p, data, (u64)count * sizeof(u16));
                    allocator->vf_0x48(data);
                }
                data = p;
                capacity = reserve;
            }
            __builtin_memcpy(data, other.data, (u64)other.count * sizeof(u16));
            count = other.count;
        }
        return *this;
    }
};
