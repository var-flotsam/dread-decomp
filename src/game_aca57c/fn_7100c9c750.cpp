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

ARRAY_DTOR(7100c9c750, UnkStaticPathId, 3, g_unk_7172795ef0)
ARRAY_DTOR(7100c9c850, UnkStaticPathId, 3, g_unk_7172795f08)
ARRAY_DTOR(7100c9c950, UnkStaticPathId, 3, g_unk_7172795f20)
ARRAY_DTOR(7100c9ca50, UnkStaticPathId, 3, g_unk_7172795f38)
ARRAY_DTOR(7100da6e50, UnkStaticPathId, 3, g_unk_71727bc5e0)
ARRAY_DTOR(7100da6f50, UnkStaticPathId, 3, g_unk_71727bc5f8)
ARRAY_DTOR(7100ec63b0, UnkStaticId, 3, g_unk_71729ce1a0)
ARRAY_DTOR(7100af4940, UnkStaticId, 12, g_unk_717275c8e0)
ARRAY_DTOR(7100af4cf0, UnkStaticId, 12, g_unk_717275c940)
ARRAY_DTOR(7100af50a0, UnkStaticId, 12, g_unk_717275c9a0)
ARRAY_DTOR(7100dda4f0, UnkStaticId, 12, g_unk_71727be228)
ARRAY_DTOR(7100dda8a0, UnkStaticId, 12, g_unk_71727be288)
ARRAY_DTOR(7100c9cd60, UnkStaticPathId, 10, g_unk_7172795f80)
ARRAY_DTOR(7100c9d070, UnkStaticPathId, 10, g_unk_7172795fd0)
ARRAY_DTOR(7100dddf50, UnkStaticPathId, 2, g_unk_71727be340)
GOT_ARRAY_DTOR(7100dd9370, UnkStaticPathId, 12, g_unk_71727be010)
GOT_ARRAY_DTOR(7100dd96f0, UnkStaticPathId, 12, g_unk_71727be070)
GOT_ARRAY_DTOR(7100dd9a70, UnkStaticPathId, 12, g_unk_71727be0d0)
GOT_ARRAY_DTOR(7100dd9df0, UnkStaticPathId, 12, g_unk_71727be130)
GOT_ARRAY_DTOR(7100dda170, UnkStaticPathId, 12, g_unk_71727be190)
GOT_ARRAY_DTOR(7100b87660, UnkStaticId, 3, g_unk_7172768948)
GOT_ARRAY_DTOR(7100b87760, UnkStaticId, 3, g_unk_7172768960)
GOT_ARRAY_DTOR(7100ca5e60, UnkStaticPathId, 10, g_unk_7172796d70)
GOT_ARRAY_DTOR(7100e1dbc0, UnkStaticId, 7, g_unk_71727c1088)
