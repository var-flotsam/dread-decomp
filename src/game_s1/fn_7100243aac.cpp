// Frees an array of owned objects (base::UnkVector's layout, as UnkOwnedList in
// include/game_s2/UnkOwnedList.h): each element is taken out, shut down through its slot 0x90
// and deleted through its slot 0x10, then the array is freed. fn_71002de984 is a relative.

#include "base/UnkVector.h"
#include "types.h"

struct UnkOwned_7100243aac {
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

struct UnkOwnedPtr_7100243aac {
    // A union member: the pointer stores get no type-based alias info, so the array is reloaded
    // after every element.
    union {
        UnkOwned_7100243aac* ptr;
    };

    __attribute__((always_inline)) ~UnkOwnedPtr_7100243aac() {
        UnkOwned_7100243aac* old = ptr;
        ptr = nullptr;
        if (old) {
            old->vf_0x90();
            old->vf_0x10();
        }
    }
};

struct UnkOwnedArray_7100243aac {
    union {
        UnkOwnedPtr_7100243aac* data;
    };
    u32 count;
    u32 capacity;
    u8 pad_0x10[0x10];
    base::UnkAllocator* allocator;
};

void fn_7100243aac(UnkOwnedArray_7100243aac* self) {
    if (self->data) {
        for (u32 i = 0; i < self->count; ++i)
            self->data[i].~UnkOwnedPtr_7100243aac();
        self->count = 0;
        self->allocator->vf_0x48(self->data);
        self->capacity = 0;
    }
}
