#pragma once

// A container whose nodes come from an embedded block pool (base/UnkCloneHelpers.h's UnkBlockPool),
// laid out like UnkPoolList but with the flags before the allocator and an optional parent for the
// pool at +0x30. Its constructor (fn_71011b250c for 0x18-byte nodes, fn_71010f8804 for 0x28-byte
// ones) sets the base's vtable, then the class's, and sizes the pool for `n` nodes. Names and layouts
// are placeholders.

#include "base/UnkBlockFreeList.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkParentPoolList {
    char* vtable;
    u32 count;
    void* head;
    void* tail;
    u32 flags;
    base::UnkAllocator* allocator;
    base::UnkAllocator* pool_parent;  // the pool's parent when set, else the allocator
    base::UnkBlockPool pool;
};
static_assert(sizeof(UnkParentPoolList) == 0x78, "UnkParentPoolList placeholder layout");

// The constructor, for nodes of `Elem` bytes and the two vtables.
template <u64 Elem, char* BaseVtable, char* Vtable>
__attribute__((always_inline)) inline void refl_ParentPoolListCtor(UnkParentPoolList* self, u32 n, u32 flags,
                                                                    base::UnkAllocator* a) {
    self->vtable = BaseVtable + 0x10;
    self->count = 0;
    self->flags = flags;
    self->head = nullptr;
    self->tail = nullptr;
    if (!a)
        a = fn_7100083990();
    self->allocator = a;
    self->pool_parent = nullptr;
    self->vtable = Vtable + 0x10;
    self->pool.Init();
    base::UnkAllocator* parent = self->pool_parent;
    self->pool.field_0x38 = 0;
    self->pool.field_0x3c = false;
    if (!parent)
        parent = self->allocator;
    u32 f = self->flags;
    self->pool.Release();
    if (n) {
        if (!parent)
            parent = fn_7100083990();
        self->pool.parent = parent;
        u64 size = (u64)n * Elem;
        fn_710008378c(&self->pool.blocks, parent->vf_0x40(size, 8, f), size, Elem);
    }
    // The pool's address, formed as an integer (as in UnkPoolList::Init).
    self->allocator = reinterpret_cast<base::UnkAllocator*>(reinterpret_cast<u64>(self) + 0x38);
}
