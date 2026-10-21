#pragma once

// A heap buffer that owns one block from an allocator (vtable via GOT 0x7101cbc2b8; destructors
// fn_7100081150/fn_7100081198, free fn_71000811e0, resize fn_7100081214). Names and layout are
// placeholders inferred from the code.

#include <new>

#include "base/UnkVector.h"
#include "types.h"

namespace base {

struct UnkBuffer {
    // Allocates `size` bytes at once, from the default allocator when none is given.
    UnkBuffer(u64 size, UnkAllocator* allocator, s32 align, u32 flags);

    virtual ~UnkBuffer();

    UnkAllocator* allocator;  // +0x8
    void* data;
    u64 size;
    s32 align;  // +0x20
    u32 flags;
};

inline UnkBuffer::UnkBuffer(u64 size, UnkAllocator* allocator, s32 align, u32 flags)
    : allocator(allocator), data(nullptr), size(size), align(align), flags(flags) {
    if (size) {
        if (!allocator)
            this->allocator = allocator = fn_7100083990();
        data = allocator->vf_0x40(size, align, flags);
    }
}

}  // namespace base
