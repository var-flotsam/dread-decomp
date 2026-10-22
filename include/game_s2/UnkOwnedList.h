#pragma once

// A growable array of owned objects (each destroyed through its vtable slot at +0x10) and the
// element type of the arrays of such lists. The list has UnkVector's layout. Neither name is
// known; both keep placeholders.

#include "base/UnkVector.h"

struct UnkOwned {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};

struct UnkOwnedPtr {
    // A union member: the pointer stores get no type-based alias info, so the list's count is
    // reloaded after every element, as in the original.
    union {
        UnkOwned* ptr;
    };
    ~UnkOwnedPtr() {
        UnkOwned* old = ptr;
        ptr = nullptr;
        if (old)
            old->vf_0x10();
    }
};

struct UnkOwnedList {
    // Also a union member, so the moves below reload the array pointer of the container they
    // are in.
    union {
        UnkOwnedPtr* data;
    };
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    // The value-initialised element of GrowAt (fn_71004a972c): an empty list that allocates with
    // flags 0xf.
    UnkOwnedList()
        : data(nullptr), count(0), capacity(0), step(10), field_0x14(true), flags(0xf), allocator(fn_7100083990()) {}
    UnkOwnedList(UnkOwnedList&& other) {
        data = other.data;
        count = other.count;
        capacity = other.capacity;
        step = other.step;
        field_0x14 = other.field_0x14;
        flags = other.flags;
        allocator = other.allocator;
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    ~UnkOwnedList() {
        if (data) {
            for (u32 i = 0; i < count; ++i)
                data[i].~UnkOwnedPtr();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};
static_assert(sizeof(UnkOwnedList) == 0x28, "UnkOwnedList layout");
