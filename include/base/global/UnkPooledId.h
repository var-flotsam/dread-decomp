#pragma once

// A string id over a given pool (the file-path pool g_unk_7101d47200, the string pool g_unk_7101d47108) as the
// music manager's records hold it: its destructor decrements and then re-tests the count. Names are
// placeholders.
#include "base/global/CStrId.h"
#include "types.h"

template <base::global::UnkStringPool** Pool>
struct UnkPooledId {
    base::global::UnkStringData* data;

    UnkPooledId(const UnkPooledId& other) : data(other.data) {
        if (&(*Pool)->empty != data)
            ++data->ref_count;
    }
    ~UnkPooledId() {
        if (base::global::UnkStringPool* pool = *Pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = *Pool;
                }
            }
            data = &pool->empty;
        }
    }
};
