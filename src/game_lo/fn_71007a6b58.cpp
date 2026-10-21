// UnkVector::GrowAt (base/UnkVector.h) for a polymorphic 0x88-byte element: grows the array
// past `index` when allowed and returns the element. New elements are value-initialised: zeroed,
// then constructed by fn_71007a8db0 (the element's implicit default constructor, out of line).
#include "base/UnkVector.h"

struct UnkElem_0x88 {
    UnkElem_0x88(UnkElem_0x88&& other);  // fn_71007abba4
    virtual void vf_0x0();
    virtual ~UnkElem_0x88();

    u8 field_0x8[0x80];
};

void fn_71007a8db0(UnkElem_0x88* elem);

namespace {

// UnkVector's private MoveTo/Reallocate for this element.
inline void Reallocate(base::UnkVector<UnkElem_0x88>* v, u32 new_capacity) {
    auto* p = static_cast<UnkElem_0x88*>(
        v->allocator->vf_0x40((u64)new_capacity * sizeof(UnkElem_0x88), 8, v->flags));
    if (v->data) {
        UnkElem_0x88* src = v->data;
        UnkElem_0x88* dst = p;
        for (u32 i = 0; i < v->count; ++i, ++dst, ++src) {
            new (dst) UnkElem_0x88(static_cast<UnkElem_0x88&&>(*src));
            src->~UnkElem_0x88();
        }
        v->allocator->vf_0x48(v->data);
    }
    v->data = p;
    v->capacity = new_capacity;
}

}  // namespace

// NON_MATCHING(minor): register allocation and scheduling only: the move loop copies the next
// source pointer after the count compare instead of before it, and the value-initialising
// loop swaps the registers of its counter and offset (the header's GrowAt gives the same code).
UnkElem_0x88& fn_71007a6b58(base::UnkVector<UnkElem_0x88>* v, u32 index) {
    if (v->field_0x14 && v->count <= index) {
        u32 cap = v->capacity;
        if (cap <= index && v->step != 0) {
            u32 grown = (index / v->step + 1) * v->step;
            if (cap < grown) {
                Reallocate(v, grown);
                cap = grown;
            }
        }
        u32 n = index + 1;
        if (cap < n)
            Reallocate(v, n);
        for (u32 i = v->count; i < n; ++i) {
            UnkElem_0x88* p = &v->data[i];
            __builtin_memset(p, 0, sizeof(UnkElem_0x88));
            fn_71007a8db0(p);
        }
        v->count = n;
    }
    return v->data[index];
}
