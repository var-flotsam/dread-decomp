// Destructors of static arrays of string ids (registered with atexit by their static
// initialisers): the elements are released last to first, each only while the pool exists.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id whose release decrements and tests the count in two steps.
template <UnkStringPool** Pool>
struct UnkStaticStrId {
    UnkStringData* data;

    ~UnkStaticStrId() {
        if (UnkStringPool* pool = *Pool) {
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
static inline void DestroyArray() {
    for (int i = N; i-- > 0;)
        Array[i].~T();
}

#define ARRAY_DTOR(addr, T, n, array)                                                          \
    extern T array[n] __attribute__((visibility("hidden")));                                   \
    void fn_##addr() { DestroyArray<T, n, array>(); }

// The same, for arrays reached through the GOT.
#define GOT_ARRAY_DTOR(addr, T, n, array)                                                      \
    extern T array[n];                                                                         \
    void fn_##addr() { DestroyArray<T, n, array>(); }

ARRAY_DTOR(7100536450, UnkStaticId, 3, g_unk_71726e2308)
