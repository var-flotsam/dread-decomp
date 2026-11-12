// Insert and copy-assignment of a growable array of reference-counted pointers (base::UnkVector's
// layout) with an out-of-line Reserve: appending grows like PushBack (including its hang);
// inserting shifts the later elements up by copying each into the next slot and releasing the
// old one. Two array types (one Reserve each) have two Insert copies each.
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

template <class Tag>
struct UnkSlotArray {
    UnkRefPtr* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

template <class Tag, void (*Reserve)(UnkSlotArray<Tag>*, u32)>
__attribute__((always_inline)) static inline void Insert(UnkSlotArray<Tag>* self, const UnkRefPtr& value,
                                                         u32 index) {
    u32 cap = self->capacity;
    u32 n = self->count;
    if (n == index) {
        u32 new_capacity = cap;
        if (cap <= n && self->step != 0)
            new_capacity = (index / self->step + 1) * self->step;
        if ((s32)new_capacity <= (s32)index)
            for (;;) {
            }
        Reserve(self, new_capacity);
        new (&self->data[self->count]) UnkRefPtr(value);
    } else {
        u32 new_capacity = cap;
        if (cap <= n && self->step != 0)
            new_capacity = (n / self->step + 1) * self->step;
        Reserve(self, new_capacity);
        for (u32 i = self->count - 1; i >= index; --i) {
            UnkRefPtr* q = &self->data[i];
            new (q + 1) UnkRefPtr(*q);
            self->data[i].~UnkRefPtr();
            if (i == 0)
                break;
        }
        new (&self->data[index]) UnkRefPtr(value);
    }
    ++self->count;
}

// Copy-assignment: releases the elements, reserves the source's capacity and appends each
// source element.
template <class Tag, void (*Reserve)(UnkSlotArray<Tag>*, u32)>
__attribute__((always_inline)) static inline UnkSlotArray<Tag>* Assign(UnkSlotArray<Tag>* self,
                                                                         const UnkSlotArray<Tag>& other) {
    if (self == &other)
        return self;
    for (u32 i = 0; i < self->count; ++i)
        self->data[i].~UnkRefPtr();
    self->count = 0;
    Reserve(self, other.capacity);
    for (const UnkRefPtr *it = other.data, *end = it + other.count; it != end; ++it) {
        u32 n = self->count;
        u32 new_capacity = self->capacity;
        if (new_capacity <= n && self->step != 0)
            new_capacity = (n / self->step + 1) * self->step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        Reserve(self, new_capacity);
        new (&self->data[self->count]) UnkRefPtr(*it);
        ++self->count;
    }
    return self;
}

struct UnkTag_71007ed25c;
struct UnkTag_710031da18;
void fn_71007ed25c(UnkSlotArray<UnkTag_71007ed25c>* self, u32 capacity);
void fn_710031da18(UnkSlotArray<UnkTag_710031da18>* self, u32 capacity);

// NON_MATCHING(minor): in the shifting loop the release call is laid out after the loop, not
// inline as in the original.
void fn_7100d407c0(UnkSlotArray<UnkTag_71007ed25c>* self, const UnkRefPtr& value, u32 index) {
    Insert<UnkTag_71007ed25c, fn_71007ed25c>(self, value, index);
}

// NON_MATCHING(minor): in the shifting loop the release call is laid out after the loop, not
// inline as in the original.
void fn_7100d409a0(UnkSlotArray<UnkTag_71007ed25c>* self, const UnkRefPtr& value, u32 index) {
    Insert<UnkTag_71007ed25c, fn_71007ed25c>(self, value, index);
}

// NON_MATCHING(minor): in the shifting loop the release call is laid out after the loop, not
// inline as in the original.
void fn_7100d84718(UnkSlotArray<UnkTag_710031da18>* self, const UnkRefPtr& value, u32 index) {
    Insert<UnkTag_710031da18, fn_710031da18>(self, value, index);
}

// NON_MATCHING(minor): in the shifting loop the release call is laid out after the loop, not
// inline as in the original.
void fn_7100d848f8(UnkSlotArray<UnkTag_710031da18>* self, const UnkRefPtr& value, u32 index) {
    Insert<UnkTag_710031da18, fn_710031da18>(self, value, index);
}

// NON_MATCHING(minor): when the source slot is null the original branches past the store of
// null into the new slot (already null); ours branches to the store.
UnkSlotArray<UnkTag_71007ed25c>* fn_7100d4065c(UnkSlotArray<UnkTag_71007ed25c>* self, const UnkSlotArray<UnkTag_71007ed25c>& other) {
    return Assign<UnkTag_71007ed25c, fn_71007ed25c>(self, other);
}

// NON_MATCHING(minor): when the source slot is null the original branches past the store of
// null into the new slot (already null); ours branches to the store.
UnkSlotArray<UnkTag_710031da18>* fn_7100d845b4(UnkSlotArray<UnkTag_710031da18>* self, const UnkSlotArray<UnkTag_710031da18>& other) {
    return Assign<UnkTag_710031da18, fn_710031da18>(self, other);
}
