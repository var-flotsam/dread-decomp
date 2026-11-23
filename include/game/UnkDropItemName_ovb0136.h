#pragma once

#include "base/global/CStrId.h"
#include "types.h"

// A string id assigned like CStrId; the old one is released in two steps (the original reloads
// `data` and the count after the decrement).
struct UnkDropItemName_ovb0136 {
    base::global::UnkStringData* data;
    UnkDropItemName_ovb0136& operator=(const UnkDropItemName_ovb0136& other) {
        if (data != other.data) {
            base::global::UnkStringData* d = other.data;
            base::global::UnkStringPool* pool = g_unk_7101d47108;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
};
