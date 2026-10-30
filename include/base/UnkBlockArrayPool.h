#pragma once

// A pool of fixed-size blocks in one allocation: the free list over the buffer, and an array with an
// entry per block. The rumble manager keeps one of 0xa8-byte blocks at +0x2180
// (src/base/input/CRumbleManager.cpp); other units have their own block sizes. (Re)creating it is
// each unit's copy of one template. Names and layouts are placeholders.

#include "base/UnkBlockFreeList.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkBlockArrayEntry {
    void* block;
    u64 field_0x8;
};

struct UnkBlockArrayPool {
    base::UnkAllocator* allocator;  // owns the buffer
    u32 flags;
    UnkBlockFreeList list;                         // +0x10
    base::UnkVector<UnkBlockArrayEntry> entries;  // +0x38
    u64 count;                                     // +0x60
};

// (Re)creates the pool with `n` blocks of `Size` bytes from `allocator` (the default one when null),
// freeing the previous buffer.
template <u64 Size>
__attribute__((always_inline)) inline void UnkBlockArrayPoolCreate(UnkBlockArrayPool* self, u64 n, u32 flags,
                                                                   base::UnkAllocator* allocator) {
    // The list's buffer, read as a word at +0x30: the original recomputes the list's address for
    // each call instead of keeping it from this read.
    if (void* buffer = *reinterpret_cast<void**>(reinterpret_cast<char*>(self) + 0x30)) {
        fn_71000837f4(&self->list);
        self->entries.Clear();
        self->allocator->vf_0x48(buffer);
        self->allocator = nullptr;
    }
    self->flags = flags;
    if (!allocator)
        allocator = fn_7100083990();
    self->allocator = allocator;
    u8* mem = static_cast<u8*>(allocator->vf_0x40(n * Size, 8, flags));
    fn_710008378c(&self->list, mem, n * Size, Size);
    self->entries.Reserve(n);
    for (u64 i = 0; i < n; ++i) {
        UnkBlockArrayEntry e = {mem + i * Size, 0};
        self->entries.PushBack(e);
    }
    self->count = n;
}
