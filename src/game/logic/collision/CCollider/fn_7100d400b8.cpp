// Assignment at an index of a growable array of pointer-sized slots (base::UnkVector's
// layout, Reserve out of line): when the array may grow (+0x14) and the index is past the end,
// it reserves (rounded up to the step), then exactly enough, fills the new slots with nulls and
// takes the new count; the slot is then assigned (out of line). Two array types (one Reserve and
// one slot assignment each), two copies each.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

template <class Tag>
struct UnkSlot {
    union {  // (the original reloads the array after each null store, as through a union)
        void* ptr;
    };

    UnkSlot() : ptr(nullptr) {}
    UnkSlot& operator=(const UnkSlot& other);  // fn_71002a5d1c, fn_710031d208
};

template <class Tag>
struct UnkSlotArray {
    UnkSlot<Tag>* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::UnkAllocator* allocator;
};

template <class Tag, void (*Reserve)(UnkSlotArray<Tag>*, u32)>
__attribute__((always_inline)) static inline void SetAt(UnkSlotArray<Tag>* self, const u32& index,
                                                        const UnkSlot<Tag>& value) {
    u32 i = index;
    if (self->growable && self->count <= i) {
        u32 c = self->capacity;
        if (c <= i && self->step != 0)
            c = (i / self->step + 1) * self->step;
        Reserve(self, c);
        u32 n = i + 1;
        Reserve(self, n);
        for (u32 k = self->count; k < n; ++k)
            new (&self->data[k]) UnkSlot<Tag>();
        self->count = n;
    }
    self->data[i] = value;
}

struct UnkTag_71007ed25c;
struct UnkTag_710031da18;
void fn_71007ed25c(UnkSlotArray<UnkTag_71007ed25c>* self, u32 capacity);
void fn_710031da18(UnkSlotArray<UnkTag_710031da18>* self, u32 capacity);

void fn_7100d400b8(UnkSlotArray<UnkTag_71007ed25c>* self, const u32& index, const UnkSlot<UnkTag_71007ed25c>& value) {
    SetAt<UnkTag_71007ed25c, fn_71007ed25c>(self, index, value);
}
void fn_7100d401bc(UnkSlotArray<UnkTag_71007ed25c>* self, const u32& index, const UnkSlot<UnkTag_71007ed25c>& value) {
    SetAt<UnkTag_71007ed25c, fn_71007ed25c>(self, index, value);
}
void fn_7100d84124(UnkSlotArray<UnkTag_710031da18>* self, const u32& index, const UnkSlot<UnkTag_710031da18>& value) {
    SetAt<UnkTag_710031da18, fn_710031da18>(self, index, value);
}
void fn_7100d84228(UnkSlotArray<UnkTag_710031da18>* self, const u32& index, const UnkSlot<UnkTag_710031da18>& value) {
    SetAt<UnkTag_710031da18, fn_710031da18>(self, index, value);
}
