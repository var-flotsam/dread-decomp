#pragma once

// String ids held as a bare pointer to the pooled string (no base class, unlike CStrId), as many
// records and arrays in game code hold them. Which engine type they are is unknown; names are
// placeholders. CStrId, UnkStrIdMemberOf (base/UnkDestructors.h) and UnkPooledStrId
// (base/UnkPooledStrId.h) are the forms with copies and assignments.

#include "base/global/CStrId.h"

// A string id whose destructor decrements the count and then tests it in a second step, so `data`
// and the count are reloaded in between (the string-id members of many destructors in game code,
// fn_71002b24e8). `Pool` selects the pool the id belongs to (the CStrId pool or the file-path
// pool). It has no constructor: the owners build it in place or copy the pointer.
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
using UnkPathIdTwoStep = UnkTwoStepStrId<&g_unk_7101d47200>;

// A string id moved by taking its pointer and leaving the other one empty (CStrId itself has no move
// constructor: it would copy and take a reference). It has no destructor: its owner releases it
// (CMinimapData's record, fn_7100e4e310).
struct UnkMovedStrId {
    base::global::UnkStringData* data;

    __attribute__((always_inline)) UnkMovedStrId(UnkMovedStrId&& other) : data(other.data) {
        other.data = &g_unk_7101d47108->empty;
    }
};

// A string id built empty: the string-id constructor inlined with no string, so only the pool check
// and the pool's empty string remain (CModelUpdaterComponent's set-up record, fn_7101170da0). It has
// no destructor: its owner releases it.
struct UnkEmptyStrId {
    base::global::UnkStringData* data;

    UnkEmptyStrId() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
};
