#include <new>

#include "base/UnkVector.h"
#include "game/UnkTU341.h"
#include "types.h"

using UnkSetVector_b2 = base::UnkVector<UnkHalf_cu>;

static inline void EraseAt(UnkSetVector_b2* self, u32 index) {
    if (index < self->count) {
        u64 i = index;
        self->data[i].vf_0x0();
        for (; i < (u32)(self->count - 1); ++i) {
            UnkHalf_cu* cur = &self->data[i];
            new (cur) UnkHalf_cu(static_cast<UnkHalf_cu&&>(cur[1]));
            (&self->data[i])[1].vf_0x0();
        }
        self->count = self->count - 1;
    }
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_710024657c(UnkSetVector_b2* self, const u32* it) {
    UnkHalf_cu* base = reinterpret_cast<UnkHalf_cu*>(self);
    u64 offset = (u64)*it * sizeof(UnkHalf_cu);
    UnkHalf_cu* p = reinterpret_cast<UnkHalf_cu*>(reinterpret_cast<char*>(base) + offset);
    EraseAt(self, (u32)(p - base));
}
