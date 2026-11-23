// Unit 56 (base::global::CRntFile): removes up to `count` bytes at the position from the contents,
// moving the rest down into a smaller block (or freeing the block when nothing is left).
// Names and layouts are placeholders.
#include <string.h>

#include "base/global/CRntFile.h"
#include "types.h"

using base::global::CRntFile;

namespace {

inline void* AllocateBlock(CRntFile* file, s64 size) {
    base::UnkAllocator* allocator = file->allocator;
    if (!allocator) {
        allocator = GetDefaultAllocator();
        file->allocator = allocator;
        if (!allocator)
            return nullptr;
    }
    return allocator->vf_0x40(size, file->alloc_align, file->alloc_flags);
}

inline void FreeBlock(CRntFile* file) {
    if (file->allocator)
        file->allocator->vf_0x48(file->data);
}

}  // namespace

void fn_710007c610(CRntFile* file, u64 count) {
    u64 position = file->position;
    u64 left = file->size - position;
    u64 removed = left < count ? left : count;
    if (removed == 0)
        return;
    u64 remaining = file->size - removed;
    if (remaining != 0) {
        u8* block = static_cast<u8*>(AllocateBlock(file, remaining));
        if (position)
            memcpy(block, file->data, position);
        u64 tail = removed + position;
        u64 tail_size = file->size - tail;
        if (tail_size)
            memcpy(block + position, file->data + tail, tail_size);
        FreeBlock(file);
        file->data = block;
    } else {
        FreeBlock(file);
        file->data = nullptr;
    }
    file->size = remaining;
}
