// Deleting destructors of three reflection classes that hold a chained hash set at 8 (bucket
// array data at 8, count at 0x10, capacity at 0x14, allocator at 0x28): each clears the set with
// its own clear function, frees the bucket array (without resetting the capacity) and then the
// object. fn_7100022518 (vtable unk_7101a19690, clear fn_7100022bd0), fn_71000217c4
// (unk_7101a19630, fn_7100021e7c) and fn_7100020a68 (unk_7101a195b0, fn_7100021128).
// fn_7100d95e40 is the complete destructor of a similar class. Called only through their vtables.

#include <new>

#include "base/UnkVector.h"
#include "types.h"

extern char unk_7101a19690[];
extern char unk_7101a19630[];
extern char unk_7101a195b0[];
void fn_7100022bd0(void* set);
void fn_7100021e7c(void* set);
void fn_7100021128(void* set);

struct UnkSetHolder_7100022518 {
    char* vtable;
    void* data;
    u32 count;
    u32 capacity;
    u8 pad_0x18[0x10];
    base::UnkAllocator* allocator;
};

template <char* Vtable, void (*Clear)(void*)>
static inline void DeleteSetHolder(UnkSetHolder_7100022518* self) {
    self->vtable = Vtable + 0x10;
    Clear(&self->data);
    if (self->data) {
        self->count = 0;
        self->allocator->vf_0x48(self->data);
    }
    operator delete(self);
}

void fn_7100022518(UnkSetHolder_7100022518* self) { DeleteSetHolder<unk_7101a19690, fn_7100022bd0>(self); }

void fn_71000217c4(UnkSetHolder_7100022518* self) { DeleteSetHolder<unk_7101a19630, fn_7100021e7c>(self); }

void fn_7100020a68(UnkSetHolder_7100022518* self) { DeleteSetHolder<unk_7101a195b0, fn_7100021128>(self); }
