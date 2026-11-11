#include <new>

#include "base/UnkVector.h"
#include "types.h"

void fn_71007b1928(void*, void*);

struct UnkSetElem_b0220c {
    void** vtable;
    u8 pad_0x8[0x38];

    UnkSetElem_b0220c(UnkSetElem_b0220c&& other) { fn_71007b1928(this, &other); }  // fn_71007b1928
    // The destructor is virtual slot 0.
    ~UnkSetElem_b0220c() { reinterpret_cast<void (**)(UnkSetElem_b0220c*)>(vtable)[0](this); }
};
static_assert(sizeof(UnkSetElem_b0220c) == 0x40, "layout");

using UnkSetVector_b0220c = base::UnkVector<UnkSetElem_b0220c>;

// UnkVector's reallocation with the old element advanced where it is destroyed.
static inline __attribute__((always_inline)) UnkSetElem_b0220c* MoveTo_b0220(UnkSetVector_b0220c* v,
                                                                           u32 new_capacity) {
    UnkSetElem_b0220c* p =
        static_cast<UnkSetElem_b0220c*>(v->allocator->vf_0x40((u64)new_capacity * sizeof(UnkSetElem_b0220c), 8, v->flags));
    if (v->data) {
        UnkSetElem_b0220c* src = v->data;
        UnkSetElem_b0220c* dst = p;
        for (u32 i = 0; i < v->count; ++i, ++dst) {
            new (dst) UnkSetElem_b0220c(static_cast<UnkSetElem_b0220c&&>(*src));
            UnkSetElem_b0220c* old = src++;
            old->~UnkSetElem_b0220c();
        }
        v->allocator->vf_0x48(v->data);
    }
    return p;
}

void fn_7100d88ba0(UnkSetVector_b0220c* self, UnkSetElem_b0220c* value, u32 index) {
    u32 cap = self->capacity;
    u32 n = self->count;
    UnkSetElem_b0220c* p;
    if (n == index) {
        u32 new_capacity = cap;
        if (cap <= n && self->step != 0)
            new_capacity = (index / self->step + 1) * self->step;
        if ((s32)new_capacity <= (s32)index)
            for (;;) {
            }
        if (cap < new_capacity) {
            p = MoveTo_b0220(self, new_capacity);
            index = self->count;
            self->data = p;
            self->capacity = new_capacity;
        } else {
            p = self->data;
        }
    } else {
        if (cap <= n && self->step != 0) {
            u32 grown = (n / self->step + 1) * self->step;
            if (cap < grown) {
                UnkSetElem_b0220c* q = MoveTo_b0220(self, grown);
                self->data = q;
                self->capacity = grown;
                n = self->count;
            }
        }
        for (u32 i = n - 1; i >= index; --i) {
            UnkSetElem_b0220c* q = &self->data[i];
            new (q + 1) UnkSetElem_b0220c(static_cast<UnkSetElem_b0220c&&>(*q));
            self->data[i].~UnkSetElem_b0220c();
            if (i == 0)
                break;
        }
        p = self->data;
    }
    new (&p[index]) UnkSetElem_b0220c(static_cast<UnkSetElem_b0220c&&>(*value));
    ++self->count;
}
