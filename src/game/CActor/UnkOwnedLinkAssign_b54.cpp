// CActor's unit (tools/tu_map.py): four identical copy-assignments of a link that either owns an
// object (state 1, the object at +0x18, released with vf_0x90 then vf_0x10) or points at one
// (state 2: +0x10 the link itself, +0x18 the object). Assigning copies the word at +0 and the
// object at +8, drops what the link held, and points it at the new object when there is one.
// One instantiation per link type. Names and layouts are placeholders.
#include "types.h"

struct UnkOwnedObject_b54a1 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // deletes the object
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
    virtual void vf_0x90();  // finalises the object
};

template <int Tag>
struct UnkOwnedLink_b54a1 {
    u32 field_0x0;
    UnkOwnedObject_b54a1* object;
    void* self;
    UnkOwnedObject_b54a1* held;
    u32 state;

    UnkOwnedObject_b54a1* Take() {
        UnkOwnedObject_b54a1* o = held;
        held = nullptr;
        return o;
    }

    void Release() {
        if (UnkOwnedObject_b54a1* o = Take()) {
            o->vf_0x90();
            o->vf_0x10();
        }
    }

    // Drops what the link holds (a switch on the state).
    __attribute__((always_inline)) void Reset() {
        switch (state) {
        case 1:
            Release();
            [[fallthrough]];
        case 2:
            state = 0;
        }
    }

    // The same written as a chain of tests (it keeps the two paths of Assign apart, as in the
    // original), then points the link at `o`.
    __attribute__((always_inline)) void Set(UnkOwnedObject_b54a1* o) {
        if (state == 2) {
            state = 0;
        } else if (state == 1) {
            Release();
            state = 0;
        }
        state = 2;
        self = this;
        held = o;
    }

    UnkOwnedLink_b54a1* Assign(const UnkOwnedLink_b54a1* other) {
        if (this != other) {
            field_0x0 = other->field_0x0;
            UnkOwnedObject_b54a1* o = other->object;
            object = o;
            if (o)
                Set(o);
            else
                Reset();
        }
        return this;
    }
};

UnkOwnedLink_b54a1<0>* fn_7100238f0c(UnkOwnedLink_b54a1<0>* self, const UnkOwnedLink_b54a1<0>* other) {
    return self->Assign(other);
}
UnkOwnedLink_b54a1<1>* fn_7100238fe0(UnkOwnedLink_b54a1<1>* self, const UnkOwnedLink_b54a1<1>* other) {
    return self->Assign(other);
}
UnkOwnedLink_b54a1<2>* fn_71002393dc(UnkOwnedLink_b54a1<2>* self, const UnkOwnedLink_b54a1<2>* other) {
    return self->Assign(other);
}
UnkOwnedLink_b54a1<3>* fn_71002394b0(UnkOwnedLink_b54a1<3>* self, const UnkOwnedLink_b54a1<3>* other) {
    return self->Assign(other);
}
