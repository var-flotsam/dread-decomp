// Unit 56 (tools/tu_map.py; base::global::CRntFile's neighbours, base/global/CRntFile.h): the line reader
// and the copy constructor. Names and layouts are placeholders.
#include <string.h>

#include "base/UnkVector.h"
#include "base/global/CRntFile.h"
#include "types.h"

using base::global::CRntFile;

// Reads a line of at most `size` bytes from the position into the zeroed buffer; "\r" ends it as a
// "\n" (the byte after it is skipped too). False when nothing was read.
bool fn_710007c9e8(CRntFile* file, char* buffer, u32 size) {
    u64 limit = size;
    memset(buffer, 0, limit);
    u64 n = 0;
    for (;;) {
        if (n == limit)
            break;
        u64 position = file->position;
        if (n + position >= file->size)
            break;
        u8 c = file->data[position + n];
        buffer[n] = c;
        ++n;
        if (c == '\n')
            break;
        if (c == '\r') {
            buffer[n - 1] = '\n';
            ++n;
            break;
        }
    }
    file->position += n;
    return n != 0;
}

// Sets the position to the start, as fn_710007c00c(file, 0, 0) does.
static inline void SeekToStart(CRntFile* file) {
    u8* data = file->data;
    if (!data)
        return;
    u64 size = file->size;
    if (size == 0)
        file->position = 0;
    else if ((u64)data > (u64)data + size)
        file->position = size;
    else
        file->position = 0;
}

// Allocates `size` bytes from the file's allocator, taking the default one on first use.
static inline void* AllocateBlock(CRntFile* file, s64 size) {
    base::UnkAllocator* allocator = file->allocator;
    if (!allocator) {
        allocator = fn_7100083990();
        file->allocator = allocator;
        if (!allocator)
            return nullptr;
    }
    return allocator->vf_0x40(size, file->alloc_align, file->alloc_flags);
}

void fn_710007beac(CRntFile* self, const CRntFile* other, u32 flags, base::UnkAllocator* allocator) {
    self->alloc_flags = flags;
    *reinterpret_cast<u64*>(self->pad_0x28) = 0;
    self->swap = false;
    if (!allocator)
        allocator = other->allocator;
    self->allocator = allocator;
    self->alloc_align = other->alloc_align;
    self->pad_0x32[0] = other->pad_0x32[0];
    self->pad_0x32[1] = other->pad_0x32[1];
    self->pad_0x32[2] = 0;
    u64 size = other->size;
    u8* block = static_cast<u8*>(AllocateBlock(self, size));
    self->data = block;
    if (block) {
        self->size = size;
        self->position = 0;
    }
    memcpy(block, other->data, other->size);
    self->size = other->size;
    SeekToStart(self);
}
