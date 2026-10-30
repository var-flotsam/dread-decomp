// Reserve of a growable array of reference-counted pointers (base::UnkVector's layout): moves
// each pointer into the new storage by copying it (taking a reference) and releasing the old
// slot, then frees the old storage. Four array types share the same body. Also the arrays'
// destructor and Clear.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

// A reference-counted object released through vf_0x10. The count is a union member (no
// type-based alias info: the original reloads the slots after the count changes).
struct UnkRefObject {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        s32 ref_count;
    };
};

struct UnkRefPtr {
    UnkRefObject* ptr = nullptr;

    UnkRefPtr() = default;
    UnkRefPtr(const UnkRefPtr& other) { *this = other; }
    UnkRefPtr& operator=(const UnkRefPtr& other) {
        UnkRefObject* p = other.ptr;
        if (p)
            ++p->ref_count;
        if (UnkRefObject* old = ptr) {
            if (--old->ref_count == 0)
                old->vf_0x10();
        }
        ptr = p;
        return *this;
    }
    ~UnkRefPtr() {
        if (UnkRefObject* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkRefPtrArray {
    UnkRefPtr* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

static inline void Reserve(UnkRefPtrArray* self, u32 capacity) {
    if (self->capacity >= capacity)
        return;
    UnkRefPtr* data = (UnkRefPtr*)self->allocator->vf_0x40((u64)capacity * sizeof(UnkRefPtr),
                                                          alignof(UnkRefPtr), self->flags);
    if (UnkRefPtr* old = self->data) {
        for (u64 i = 0; (u32)i < self->count; ++i) {
            new (&data[i]) UnkRefPtr(old[i]);
            old[i].~UnkRefPtr();
        }
        self->allocator->vf_0x48(self->data);
    }
    self->data = data;
    self->capacity = capacity;
}

// NON_MATCHING(minor): the original gives the null-pointer and release paths their own copies of
// the loop latch; ours shares one latch.
void fn_7100afe1d0(UnkRefPtrArray* self, u32 capacity) { Reserve(self, capacity); }

// NON_MATCHING(minor): the original gives the null-pointer and release paths their own copies of
// the loop latch; ours shares one latch.
void fn_7100d5cb38(UnkRefPtrArray* self, u32 capacity) { Reserve(self, capacity); }

// NON_MATCHING(minor): the original gives the null-pointer and release paths their own copies of
// the loop latch; ours shares one latch.
void fn_7100da2900(UnkRefPtrArray* self, u32 capacity) { Reserve(self, capacity); }

// NON_MATCHING(minor): the original gives the null-pointer and release paths their own copies of
// the loop latch; ours shares one latch.
void fn_7100e169a4(UnkRefPtrArray* self, u32 capacity) { Reserve(self, capacity); }

// Destructor: releases each pointer and frees the storage. Four identical copies.
static inline void Free(UnkRefPtrArray* self) {
    if (self->data) {
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].~UnkRefPtr();
        self->count = 0;
        self->allocator->vf_0x48(self->data);
        self->capacity = 0;
    }
}

void fn_7100d403b4(UnkRefPtrArray* self) { Free(self); }
void fn_7100d84420(UnkRefPtrArray* self) { Free(self); }
void fn_7100da2060(UnkRefPtrArray* self) { Free(self); }
// A copy in another unit, on an array after a word.
struct UnkWordRefPtrArray {
    u64 field_0x0;
    UnkRefPtrArray array;  // 0x8
};

void fn_71007051f0(UnkWordRefPtrArray* self) { Free(&self->array); }
void fn_7100de49cc(UnkRefPtrArray* self) { Free(self); }

// Clear: releases each pointer, keeping the storage. Three identical copies.
static inline void Clear(UnkRefPtrArray* self) {
    for (u64 i = 0; i < self->count; ++i)
        self->data[i].~UnkRefPtr();
    self->count = 0;
}

void fn_7100d3fecc(UnkRefPtrArray* self) { Clear(self); }
void fn_7100d83f38(UnkRefPtrArray* self) { Clear(self); }
void fn_7100da2774(UnkRefPtrArray* self) { Clear(self); }
