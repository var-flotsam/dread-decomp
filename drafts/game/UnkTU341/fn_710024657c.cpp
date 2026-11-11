#include "base/UnkVector.h"
#include "types.h"

// The callback set (src/game_71001e2acc/fn_71002475b0.cpp): its in-place destructor is the first
// virtual and its move constructor is fn_71002458a4.
struct UnkSet_b2 {
    virtual void vf_0x0();
    u8 pad_0x8[0x38];
};
static_assert(sizeof(UnkSet_b2) == 0x40, "UnkSet_b2 layout");

void fn_71002458a4(void* dst, void* src);

using UnkSetVector_b2 = base::UnkVector<UnkSet_b2>;

static inline void EraseAt(UnkSetVector_b2* self, u32 index) {
    if (index < self->count) {
        u64 i = index;
        self->data[i].vf_0x0();
        for (; i < (u32)(self->count - 1); ++i) {
            UnkSet_b2* cur = &self->data[i];
            fn_71002458a4(cur, &cur[1]);
            (&self->data[i])[1].vf_0x0();
        }
        self->count = self->count - 1;
    }
}

void fn_710024657c(UnkSetVector_b2* self, const u32* it) {
    UnkSet_b2* base = reinterpret_cast<UnkSet_b2*>(self);
    u64 offset = (u64)*it * sizeof(UnkSet_b2);
    UnkSet_b2* p = reinterpret_cast<UnkSet_b2*>(reinterpret_cast<char*>(base) + offset);
    EraseAt(self, (u32)(p - base));
}
