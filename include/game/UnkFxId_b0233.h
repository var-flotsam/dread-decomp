#pragma once

#include "base/global/CStrId.h"
#include "types.h"

// A string id that is never null: the copy and release test only against the pool's empty id.
struct UnkFxId_b0233 {
    base::global::UnkStringData* data;

    UnkFxId_b0233(const UnkFxId_b0233& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkFxId_b0233() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data != &pool->empty) {
                if (--data->ref_count == 0)
                    StringPool_Release(pool, data);
            }
        }
    }
};
