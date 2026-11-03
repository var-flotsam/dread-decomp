// More releases of the inline-storage arena (copies of fn_710009fef0 in
// src/base/UnkUnit25/UnkInlineArena_b42.cpp, for 0x100 and 0x200 bytes of storage): when the nesting
// count drops to zero, the cursor rewinds to the storage start if the pointer lies below it.
// Names and layouts are placeholders.
#include "types.h"

template <u32 N>
struct UnkInlineArena_b54a1 {
    u64 field_0x0;
    u8 storage[N];
    u8* cursor;
    u32 depth;

    __attribute__((always_inline)) void Release(u8* p) {
        u32 d = depth;
        if (d == 1 && storage <= p && p < cursor)
            cursor = storage;
        depth = d - 1;
    }
};

void fn_710001f374(UnkInlineArena_b54a1<0x100>* self, u8* p) { self->Release(p); }
void fn_7100075874(UnkInlineArena_b54a1<0x200>* self, u8* p) { self->Release(p); }
