// The default allocator's sub-allocators: allocate, free and look up over one fixed range.
#include "base/UnkDefaultAllocator.h"

using base::UnkSubAllocator;

// Slot 0x40. Nothing comes from a sub-allocator before it is set up or while field_0x31 is set.
void* fn_71000813e4(UnkSubAllocator* self, u64 size, u64 align, u32 flags) {
    if (!self->ready || self->field_0x31)
        return nullptr;
    if (align == 8)
        return fn_7100081860(self->space, size);
    return fn_71000829f0(self->space, align, size);
}

// Slot 0x48.
void fn_7100081420(UnkSubAllocator* self, void* ptr) {
    if (ptr && self->vf_0x58(ptr)) {
        self->vf_0xd8(ptr);
        fn_7100082370(self->space, ptr);
    }
}

// Slot 0x58: whether ptr lies in the range.
bool fn_71000814d4(UnkSubAllocator* self, void* ptr) {
    return ptr && self->begin <= ptr && ptr < self->end;
}

// The largest block that can be allocated now, found by trying (a binary search up to twice the
// free size).
u64 fn_7100081504(UnkSubAllocator* self) {
    u64 lo = 0;
    u64 hi = self->vf_0x90() * 2;
    while (lo < hi) {
        u64 mid = (lo + hi) / 2;
        void* p = fn_7100081860(self->space, mid);
        if (p) {
            fn_7100082370(self->space, p);
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}
