// Unit 341 (CActor): moves an owned-object slot into an array at the given index, shifting the later
// ones up: CRntVector::Insert written out. Names and layouts are placeholders.
#include <memory>

#include "base/global/CRntVector.h"
#include "types.h"

// An object the array owns; released (vf_0x90) and then deleted (vf_0x10) by its slot.
struct UnkOwnedObject_l243c24 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
};

// Releases (vf_0x90) and then deletes (vf_0x10) an owned object.
struct UnkOwnedObjectDeleter_l243c24 {
    void operator()(UnkOwnedObject_l243c24* p) const {
        p->vf_0x90();
        p->vf_0x10();
    }
};

// An owning pointer to one of those objects.
using UnkOwnedSlot_l243c24 = std::unique_ptr<UnkOwnedObject_l243c24, UnkOwnedObjectDeleter_l243c24>;

// Allocates new_capacity slots and moves the slots over, freeing the old array.
static inline UnkOwnedSlot_l243c24* MoveTo(base::global::CRntVector<UnkOwnedSlot_l243c24>* v, u32 new_capacity) {
    UnkOwnedSlot_l243c24* p = static_cast<UnkOwnedSlot_l243c24*>(
        v->allocator->vf_0x40((u64)new_capacity * sizeof(UnkOwnedSlot_l243c24), 8, v->flags));
    if (v->data) {
        UnkOwnedSlot_l243c24* src = v->data;
        UnkOwnedSlot_l243c24* dst = p;
        for (u32 i = 0; i < v->count; ++i, ++dst, ++src) {
            new (dst) UnkOwnedSlot_l243c24(static_cast<UnkOwnedSlot_l243c24&&>(*src));
            src->~unique_ptr();
        }
        v->allocator->vf_0x48(v->data);
    }
    return p;
}

void fn_7100243c24(base::global::CRntVector<UnkOwnedSlot_l243c24>* v, UnkOwnedSlot_l243c24&& value, u32 index) {
    using Slot = UnkOwnedSlot_l243c24;
    u32 cap = v->capacity;
    u32 n = v->count;
    Slot* p;
    if (n == index) {
        u32 new_capacity = cap;
        if (cap <= n && v->step != 0)
            new_capacity = (index / v->step + 1) * v->step;
        if ((s32)new_capacity <= (s32)index)
            for (;;) {
            }
        if (cap < new_capacity) {
            p = MoveTo(v, new_capacity);
            index = v->count;
            v->data = p;
            v->capacity = new_capacity;
        } else {
            p = v->data;
        }
    } else {
        if (cap <= n && v->step != 0) {
            u32 grown = (n / v->step + 1) * v->step;
            if (cap < grown) {
                Slot* q = MoveTo(v, grown);
                v->data = q;
                v->capacity = grown;
                n = v->count;
            }
        }
        for (u32 i = n - 1; i >= index; --i) {
            Slot* q = &v->data[i];
            new (q + 1) Slot(static_cast<Slot&&>(*q));
            v->data[i].~unique_ptr();
            if (i == 0)
                break;
        }
        p = v->data;
    }
    new (&p[index]) Slot(static_cast<Slot&&>(value));
    ++v->count;
}
