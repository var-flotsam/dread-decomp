// Destructors (complete and deleting) of a class (vtable unk_7101a1cda0) with a member at 0x38
// (destroyed by fn_7100014e88) above a base (vtable unk_7101a1cd10) holding an array at 0x10
// (data, count at 0x18, capacity at 0x1c, allocator at 0x30): fn_710014a86c and fn_710014a8cc.
// The deleting one frees the array without resetting its capacity. fn_71006fc63c is the same for
// a larger class.

#include <new>

#include "base/UnkVector.h"
#include "types.h"

extern char unk_7101a1cda0[];
extern char unk_7101a1cd10[];
void fn_7100014e88(void* member);

struct UnkArrayHolder_710014a86c {
    char* vtable;
    u64 field_0x8;
    void* data;
    u32 count;
    u32 capacity;
    u8 pad_0x20[0x10];
    base::UnkAllocator* allocator;
    u8 member[0x8];
};

template <bool Deleting>
__attribute__((always_inline)) static inline void Destroy(UnkArrayHolder_710014a86c* self) {
    self->vtable = unk_7101a1cda0 + 0x10;
    fn_7100014e88(self->member);
    self->vtable = unk_7101a1cd10 + 0x10;
    if (self->data) {
        self->count = 0;
        self->allocator->vf_0x48(self->data);
        if (!Deleting)
            self->capacity = 0;
    }
}

void fn_710014a86c(UnkArrayHolder_710014a86c* self) { Destroy<false>(self); }

void fn_710014a8cc(UnkArrayHolder_710014a86c* self) {
    Destroy<true>(self);
    operator delete(self);
}
