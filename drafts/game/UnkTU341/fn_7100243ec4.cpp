// Unit 341 (CActor): the owned-object slot at an index of an array, growing the array up to it with
// empty slots: CRntVector::GrowAt written out. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

// An object the array owns; released (vf_0x90) and then deleted (vf_0x10) by its slot.
struct UnkOwnedObject_l243ec4 {
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

// An owning pointer to one of those objects.
struct UnkOwnedSlot_l243ec4 {
    UnkOwnedObject_l243ec4* ptr;

    UnkOwnedSlot_l243ec4() : ptr(nullptr) {}
    UnkOwnedSlot_l243ec4(UnkOwnedSlot_l243ec4&& o) {
        UnkOwnedObject_l243ec4* moved = o.ptr;
        o.ptr = nullptr;
        ptr = moved;
    }
    ~UnkOwnedSlot_l243ec4() {
        UnkOwnedObject_l243ec4* old = ptr;
        ptr = nullptr;
        if (old) {
            old->vf_0x90();
            old->vf_0x10();
        }
    }
};

UnkOwnedSlot_l243ec4* fn_7100243ec4(base::global::CRntVector<UnkOwnedSlot_l243ec4>* self, u32 index) {
    if (self->field_0x14 && self->count <= index) {
        // First to the next multiple of the step, then to exactly index + 1 slots.
        u32 cap = self->capacity;
        if (cap <= index && self->step != 0) {
            u32 grown = (index / self->step + 1) * self->step;
            if (cap < grown) {
                self->Reserve(grown);
                cap = grown;
            }
        }
        u32 n = index + 1;
        if (cap < n)
            self->Reserve(n);
        for (u32 i = self->count; i < n; ++i)
            new (&self->data[i]) UnkOwnedSlot_l243ec4();
        self->count = n;
    }
    return &self->data[index];
}
