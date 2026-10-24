// An array's Truncate (as in fn_7100ec4fd4 over 0x20-byte records,
// src/game_aca57c/fn_7100b607f0.cpp): the records past the new count (0x10 bytes, string ids at
// 0 and 8) are destroyed, the later id first, and the count is set.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id whose destructor decrements and tests in two steps (the original reloads the id
// after the decrement).
struct UnkReleasedStrId {
    UnkStringData* data;

    __attribute__((always_inline)) ~UnkReleasedStrId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
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
};

struct UnkIdPair10 {
    UnkReleasedStrId a;
    UnkReleasedStrId b;
    };

struct UnkIdPair10Array {
    UnkIdPair10* data;
    u32 count;
};

__attribute__((always_inline)) static inline void Truncate(UnkIdPair10Array* self, u32 n) {
    for (u64 i = n; i < self->count; ++i)
        self->data[i].~UnkIdPair10();
    self->count = n;
}

void fn_71002db388(UnkIdPair10Array* self, u32 n) { Truncate(self, n); }
