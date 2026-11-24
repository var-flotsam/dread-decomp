// SCameraRailNode's unit (tools/tu_map.py): moves an element (0x50 bytes: vtable, an array, a flag, six
// words) into an array of them at an index, shifting the later elements up one place (growing the array
// first when it is full): CRntVector::Insert written out. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/SCameraRailNode.h"
#include "types.h"

using base::IAllocator;

// As src/game/SCameraRailNode/fn_7100c073f0.cpp defines them.
struct UnkMovedArray {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    IAllocator* allocator;

    UnkMovedArray(UnkMovedArray&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
};

struct UnkElem50 {
    virtual void vf_0x0();
    virtual ~UnkElem50();

    UnkMovedArray field_0x8;
    bool field_0x30;
    u32 field_0x34[6];

    UnkElem50(UnkElem50&& other)
        : field_0x8(static_cast<UnkMovedArray&&>(other.field_0x8)), field_0x30(other.field_0x30), field_0x34() {}
};
static_assert(sizeof(UnkElem50) == 0x50, "UnkElem50 layout");

using UnkElems_lc0a480 = base::global::CRntVector<UnkElem50>;

// Allocates new_capacity elements and moves the elements over, freeing the old array.
static inline UnkElem50* MoveTo(UnkElems_lc0a480* v, u32 new_capacity) {
    UnkElem50* p = static_cast<UnkElem50*>(v->allocator->vf_0x40((u64)new_capacity * sizeof(UnkElem50), 8, v->flags));
    if (UnkElem50* src = v->data) {
        UnkElem50* dst = p;
        for (u32 i = 0; i < v->count; ++i, ++dst, ++src) {
            new (dst) UnkElem50(static_cast<UnkElem50&&>(*src));
            src->~UnkElem50();
        }
        v->allocator->vf_0x48(v->data);
    }
    return p;
}

void fn_7100c0a480(UnkElems_lc0a480* self, UnkElem50* value, u32 index) {
    u32 cap = self->capacity;
    u32 n = self->count;
    if (n == index) {
        UnkElem50* p;
        u32 new_capacity = cap;
        if (cap <= n && self->step != 0)
            new_capacity = (index / self->step + 1) * self->step;
        if ((s32)new_capacity <= (s32)index)
            for (;;) {
            }
        if (cap < new_capacity) {
            p = MoveTo(self, new_capacity);
            index = self->count;
            self->data = p;
            self->capacity = new_capacity;
        } else {
            p = self->data;
        }
        new (&p[index]) UnkElem50(static_cast<UnkElem50&&>(*value));
    } else {
        if (cap <= n && self->step != 0) {
            u32 grown = (n / self->step + 1) * self->step;
            if (cap < grown) {
                UnkElem50* q = MoveTo(self, grown);
                n = self->count;
                self->data = q;
                self->capacity = grown;
            }
        }
        for (u32 i = n - 1; i >= index; --i) {
            UnkElem50* q = &self->data[i];
            new (q + 1) UnkElem50(static_cast<UnkElem50&&>(*q));
            self->data[i].~UnkElem50();
            if (i == 0)
                break;
        }
        new (&self->data[index]) UnkElem50(static_cast<UnkElem50&&>(*value));
    }
    ++self->count;
}
