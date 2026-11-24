// Two copies of an array's Truncate: the records past the new count (0x20 bytes, string ids at
// 0 and 8) are destroyed, the later id first, and the count is set.
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkIdPair20 {
    UnkStrIdTwoStep a;
    UnkStrIdTwoStep b;
    u64 field_0x10[2];
};

struct UnkIdPair20Array {
    UnkIdPair20* data;
    u32 count;
};

__attribute__((always_inline)) static inline void Truncate(UnkIdPair20Array* self, u32 n) {
    for (u64 i = n; i < self->count; ++i)
        self->data[i].~UnkIdPair20();
    self->count = n;
}

void CPopUpComposition_TruncateStrIdPairArray(UnkIdPair20Array* self, u32 n) { Truncate(self, n); }
void fn_7100ec4fd4(UnkIdPair20Array* self, u32 n) { Truncate(self, n); }
