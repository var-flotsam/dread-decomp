// Appends a 64-byte element (a 4x4 float matrix, copied word by word) to a fixed-capacity
// array. Returns 0 when it is full.
#include "types.h"

typedef u32 __attribute__((may_alias)) a_u32;

struct UnkElem_0x40 {
    a_u32 words[16];
};

struct UnkFixedArray_71000155cc {
    u8 pad_0x0[8];
    UnkElem_0x40* begin;
    u8 pad_0x10[8];
    u32 capacity;
    u8 pad_0x1c[0x14];
    UnkElem_0x40* end;
};

s32 fn_71000155cc(UnkFixedArray_71000155cc* self, const UnkElem_0x40* e) {
    UnkElem_0x40* limit = self->begin + self->capacity;
    UnkElem_0x40* d = self->end;
    if (d == limit)
        return 0;
    for (int i = 0; i < 16; ++i)
        d->words[i] = e->words[i];
    self->end += 1;
    return 1;
}
