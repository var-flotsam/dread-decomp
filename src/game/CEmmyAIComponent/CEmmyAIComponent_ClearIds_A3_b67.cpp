// CEmmyAIComponent's unit (tools/tu_map.py unit 811): resets every string id of the list at +0x2a5a8
// (count, then the ids from +0x2a5ac) to the empty string, releasing the old ones, and empties the list;
// with `refresh`, also resets the object at +0x2a300 (fn_71002b2638) and passes it on
// (fn_710110f8ac). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

void fn_71002b2638(void* object);
void fn_710110f8ac(void* self, void* object);

static inline __attribute__((always_inline)) void ResetId_A3_b67(UnkStringData*& data) {
    UnkStringPool* pool = g_unk_7101d47108;
    if (pool) {
        if (data && data != &pool->empty) {
            --data->ref_count;
            if (data->ref_count == 0) {
                fn_71000803a4(pool, data);
                pool = g_unk_7101d47108;
            }
        }
        data = &pool->empty;
    }
}

void fn_71005553a8(char* self, bool refresh) {
    u32& count = *reinterpret_cast<u32*>(self + 0x2a5a8);
    UnkStringData** ids = reinterpret_cast<UnkStringData**>(self + 0x2a5ac);
    for (u64 i = 0; i < count; ++i)
        ResetId_A3_b67(ids[i]);
    count = 0;
    if (refresh) {
        void* object = self + 0x2a300;
        fn_71002b2638(object);
        fn_710110f8ac(self, object);
    }
}
