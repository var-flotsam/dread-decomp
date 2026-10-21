#pragma once

// A string id whose destructor decrements the count and then tests it in a second step, so `data`
// and the count are reloaded in between (the string-id members of many destructors in game code).
// `Pool` selects the pool the id belongs to (the CStrId pool or the file-path pool).

#include "base/global/CStrId.h"

template <base::global::UnkStringPool** Pool>
struct UnkTwoStepStrId {
    base::global::UnkStringData* data;

    ~UnkTwoStepStrId() {
        if (base::global::UnkStringPool* pool = *Pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = *Pool;
                }
            }
            data = &pool->empty;
        }
    }
};

using UnkStrIdTwoStep = UnkTwoStepStrId<&g_unk_7101d47108>;
