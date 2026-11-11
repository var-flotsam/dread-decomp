// CMinimapData's unit: reflected collection erase (slot 0x128) for a base::UnkVector of 0x138-byte records with
// an out-of-line destructor (fn_7100e4e310) and move constructor (fn_7100e51078): the record an index iterator
// names is destroyed, each later one moved down one and the moved-from one destroyed. The index goes through a
// byte offset from the vector itself (build/b67/b67a6.md). Names are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU2563.h"
#include "types.h"

struct alignas(8) UnkRecord138_A6_b76 {
    u8 data[0x138];
    ~UnkRecord138_A6_b76() { fn_7100e4e310(reinterpret_cast<char*>(this)); }
};

static inline __attribute__((always_inline)) void EraseAt_A6_b76(base::UnkVector<UnkRecord138_A6_b76>* self,
                                                                 u32 index) {
    if (index >= self->count)
        return;
    self->data[index].~UnkRecord138_A6_b76();
    for (u64 i = index; i < self->count - 1; ++i) {
        UnkRecord138_A6_b76* src = &self->data[i] + 1;
        fn_7100e51078(src - 1, src);
        (&self->data[i] + 1)->~UnkRecord138_A6_b76();
    }
    self->count = self->count - 1;
}

void fn_7100e61f8c(base::UnkVector<UnkRecord138_A6_b76>* self, const u32* it) {
    UnkRecord138_A6_b76* base = reinterpret_cast<UnkRecord138_A6_b76*>(self);
    u64 offset = (u64)*it * sizeof(UnkRecord138_A6_b76);
    UnkRecord138_A6_b76* p = reinterpret_cast<UnkRecord138_A6_b76*>(reinterpret_cast<char*>(base) + offset);
    EraseAt_A6_b76(self, (u32)(p - base));
}
