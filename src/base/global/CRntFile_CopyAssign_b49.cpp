// base::global::CRntFile (include/base/global/CRntFile.h), unit 56: the copy assignment
// (fn_710007e258). The block is released unless it is borrowed (+0x34): through its owner (+0x28,
// slot 0x8) when there is one, else back to the allocator. A block of the other's size is then
// allocated (fn_710007bf9c, inline), the contents copied, and the position moved to the start
// (fn_710007c00c, inline). Names and layouts are placeholders.
#include <string.h>

#include "base/global/CRntFile.h"
#include "types.h"

using base::global::CRntFile;

namespace {

struct UnkBlockOwner_b49a1 {
    virtual void vf_0x0();
    virtual void vf_0x8(void* block);
};

// CRntFile's view with the members its pad hides: the block's owner and the borrowed flag.
struct UnkRntFileView_b49a1 {
    base::UnkAllocator* allocator;
    u64 position;
    u32 alloc_flags;
    u8* data;
    u64 size;
    UnkBlockOwner_b49a1* owner;  // +0x28
    u8 alloc_align;              // +0x30
    bool swap;
    u8 pad_0x32[2];
    bool borrowed;               // +0x34
};

// As src/base/global/CRntFile.cpp.
inline void* AllocateBlock(CRntFile* file, s64 size) {
    base::UnkAllocator* allocator = file->allocator;
    if (!allocator) {
        allocator = fn_7100083990();
        file->allocator = allocator;
        if (!allocator)
            return nullptr;
    }
    return allocator->vf_0x40(size, file->alloc_align, file->alloc_flags);
}

inline bool Allocate(CRntFile* file, s64 size) {
    u8* block = static_cast<u8*>(AllocateBlock(file, size));
    file->data = block;
    if (block) {
        file->size = size;
        file->position = 0;
    }
    return block != nullptr;
}

inline void Rewind(CRntFile* file) {
    u8* data = file->data;
    if (!data)
        return;
    u64 offset = 0;
    u64 size = file->size;
    if (offset >= size) {
        file->position = size;
        return;
    }
    if ((u64)data + offset > (u64)data + size)
        file->position = size;
    else
        file->position = offset;
}

}  // namespace

CRntFile* fn_710007e258(CRntFile* self, const CRntFile* other) {
    if (self != other) {
        UnkRntFileView_b49a1* view = reinterpret_cast<UnkRntFileView_b49a1*>(self);
        if (!view->borrowed && view->data) {
            if (view->owner) {
                view->owner->vf_0x8(view->data);
                view->owner = nullptr;
            } else if (view->allocator) {
                view->allocator->vf_0x48(view->data);
            }
            view->data = nullptr;
        }
        Allocate(self, other->size);
        memcpy(self->data, other->data, other->size);
        self->size = other->size;
        Rewind(self);
    }
    return self;
}
