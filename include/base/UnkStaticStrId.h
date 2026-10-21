#pragma once

// Static arrays of string ids, destroyed by functions their static initialisers register with
// atexit: the elements are released last to first, each only while the pool exists.

#include "base/global/CStrId.h"
#include "types.h"

namespace base {

// A string id whose release decrements and tests the count in two steps.
template <global::UnkStringPool** Pool>
struct UnkStaticStrId {
    global::UnkStringData* data;

    ~UnkStaticStrId() {
        if (global::UnkStringPool* pool = *Pool) {
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
using UnkStaticPathId = UnkStaticStrId<&g_unk_7101d47200>;
using UnkStaticId = UnkStaticStrId<&g_unk_7101d47108>;

template <class T, int N, T (&Array)[N]>
__attribute__((always_inline)) inline void DestroyArray() {
    for (int i = N; i-- > 0;)
        Array[i].~T();
}

}  // namespace base

// The array's destructor, fn_<addr>; GOT_ARRAY_DTOR for arrays reached through the GOT.
#define ARRAY_DTOR(addr, T, n, array)                                                          \
    extern T array[n] __attribute__((visibility("hidden")));                                   \
    void fn_##addr() { base::DestroyArray<T, n, array>(); }

#define GOT_ARRAY_DTOR(addr, T, n, array)                                                      \
    extern T array[n];                                                                         \
    void fn_##addr() { base::DestroyArray<T, n, array>(); }
