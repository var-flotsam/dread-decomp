// Deleting destructor of a reflection class holding a chained hash set at 8 (as in fn_7100022518,
// src/game_s1/fn_7100022518.cpp): vtable unk_7101a22398, clear fn_71002f6ac0.

#include <new>

#include "base/UnkVector.h"
#include "types.h"

extern char unk_7101a22398[];
void fn_71002f6ac0(void* set);

struct UnkSetHolder_71002f7850 {
    char* vtable;
    void* data;
    u32 count;
    u32 capacity;
    u8 pad_0x18[0x10];
    base::UnkAllocator* allocator;
};

void fn_71002f7850(UnkSetHolder_71002f7850* self) {
    self->vtable = unk_7101a22398 + 0x10;
    fn_71002f6ac0(&self->data);
    if (self->data) {
        self->count = 0;
        self->allocator->vf_0x48(self->data);
    }
    operator delete(self);
}
