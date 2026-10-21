// The string pool's heap setup: a function-local static heap, published through
// g_unk_7101d58d40, with a fixed number of blocks per size class.
#include "base/global/UnkStringHeap.h"
#include "base/global/UnkStringPool.h"

using base::global::UnkStringHeap;

void fn_710007fdb4() {
    static UnkStringHeap s_heap;
    g_unk_7101d58d40 = &s_heap;
    // Stored one by one, as the original does (no constant-pool copy).
    u32 counts[6];
    counts[0] = 25000;
    counts[1] = 24000;
    counts[2] = 40000;
    counts[3] = 135000;
    counts[4] = 1000;
    counts[5] = 200;
    fn_7100083f74(&s_heap, counts);
}
