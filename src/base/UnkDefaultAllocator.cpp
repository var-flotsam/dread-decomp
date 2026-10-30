// The default allocator's own functions: construction, the memory split between its four
// sub-allocators, and the slots that look a pointer up in them (newest sub-allocator first).
#include "base/UnkBlockFreeList.h"
#include "base/UnkDefaultAllocator.h"

using base::UnkDefaultAllocator;
using base::UnkSubAllocator;

// Set to the start of the memory the default allocator was given.
extern char* g_unk_7101d58e80;

namespace base {

// Inline here so that each of fn_7100082c38, fn_7100082cc8 and fn_7100082d1c below is one
// function, as in the original.
inline UnkDefaultAllocator::UnkDefaultAllocator() {
    fn_71000668f8(&mutex);
    subs[0] = &sub0;
    subs[1] = &sub1;
    subs[2] = &sub2;
    subs[3] = &sub3;
    last = 2;
}

inline UnkDefaultAllocator::~UnkDefaultAllocator() {
    fn_7100066918(&mutex);
}

}  // namespace base

// NON_MATCHING(minor): register allocation; the original computes the mutex's address before
// constructing the sub-allocators and keeps it in a callee-saved register.
void fn_7100082c38(UnkDefaultAllocator* self) {
    new (self) UnkDefaultAllocator();
}

void fn_7100082cc8(UnkDefaultAllocator* self) {
    self->~UnkDefaultAllocator();
}

void fn_7100082d1c(UnkDefaultAllocator* self) {
    self->~UnkDefaultAllocator();
    operator delete(self);
}

// Slot 0x10: the memory is split at fixed offsets (150 MiB, 512 MiB, about 573 MiB and 20 MiB);
// the end and the name are not used.
void fn_7100082d78(UnkDefaultAllocator* self, char* begin, char* end, const char* name, u64 arg) {
    char* extra_end = begin + 0x9600000;
    g_unk_7101d58e80 = begin;
    fn_7100081330(&self->sub2, begin, extra_end, "Extra", arg);
    fn_7100081380(&self->sub2, true);
    char* pack_end = begin + 0x29600000;
    fn_7100081330(&self->sub3, extra_end, pack_end, "Pack", arg);
    char* main_end = begin + 0x4d300000;
    fn_7100081330(&self->sub0, pack_end, main_end, "Main", arg);
    char* secondary_end = begin + 0x4e700000;
    fn_7100081330(&self->sub1, main_end, secondary_end, "Secondary", arg);
}

// Slot 0x50: the usable size of an allocation, from the sub-allocator that owns it.
u64 fn_7100082ea0(UnkDefaultAllocator* self, void* ptr) {
    if (!ptr)
        return 0;
    UnkSubAllocator* s;
    if ((s = self->subs[3])->vf_0x58(ptr) || (s = self->subs[2])->vf_0x58(ptr) ||
        (s = self->subs[1])->vf_0x58(ptr) || (s = self->subs[0])->vf_0x58(ptr))
        return s->vf_0x50(ptr);
    return 0;
}

// Slot 0x48: free.
void fn_71000832d8(UnkDefaultAllocator* self, void* ptr) {
    fn_710006691c(&self->mutex);
    if (ptr) {
        for (s32 i = self->last; i >= 0; --i) {
            UnkSubAllocator* s = self->subs[i];
            if (s->vf_0x58(ptr)) {
                s->vf_0x48(ptr);
                break;
            }
        }
    }
    fn_7100066920(&self->mutex);
}

// Slot 0x88: the total over the sub-allocators.
u32 fn_7100083374(UnkDefaultAllocator* self) {
    u32 total = 0;
    for (s32 i = 0; i < 4; ++i)
        total += self->subs[i]->vf_0x88();
    return total;
}

// Slot 0x58: whether any sub-allocator owns ptr.
bool fn_71000835a0(UnkDefaultAllocator* self, void* ptr) {
    return self->subs[0]->vf_0x58(ptr) || self->subs[1]->vf_0x58(ptr) || self->subs[2]->vf_0x58(ptr) ||
           self->subs[3]->vf_0x58(ptr);
}

// ---- fn_710008378c: a block free list over a buffer (base/UnkBlockFreeList.h) ----

// Sets the list up over `size` bytes of `buffer` in blocks of `elem` bytes: the buffer is cleared and
// each block starts with the (1-based) index of the block after it.
// NON_MATCHING(minor): same behaviour; the original stores the count and buffer as a pair after
// the clear word and the sizes (ours stores the buffer first), and keeps the next index in its own
// register from 1 (ours adds 1 to a counter from 0); 104 bytes either way.
void fn_710008378c(UnkBlockFreeList* list, void* buffer, u64 size, u64 elem) {
    list->field_0x10 = 0;
    list->size = size;
    list->elem = elem;
    list->count = size / elem;
    list->buffer = buffer;
    __builtin_memset(buffer, 0, size);
    if (list->count) {
        u64 offset = 0;
        u64 i = 1;
        do {
            // Stored bytewise: the list's count and buffer are reread for each block.
            __builtin_memcpy(static_cast<char*>(list->buffer) + offset, &i, sizeof(i));
            offset += elem;
        } while (i++ < list->count);
    }
}
