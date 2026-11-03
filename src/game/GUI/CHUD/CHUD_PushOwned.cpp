// GUI::CHUD unit: appends an owned object to an array of owning pointers (base::UnkVector's
// EmplaceBack): the pointer is taken over and the moved-from slots destroy what they still hold
// through vtable slot 0x10. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkHudOwned {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};

struct UnkHudOwnedPtr {
    // A union member: the pointer stores get no type-based alias info (the array's fields are
    // reloaded after them, as in the original).
    union {
        UnkHudOwned* ptr;
    };
    UnkHudOwnedPtr(UnkHudOwnedPtr&& other) {
        UnkHudOwned* p = other.ptr;
        other.ptr = nullptr;
        ptr = p;
    }
    ~UnkHudOwnedPtr() {
        UnkHudOwned* old = ptr;
        ptr = nullptr;
        if (old)
            old->vf_0x10();
    }
};

void fn_71011e7700(base::UnkVector<UnkHudOwnedPtr>* self, UnkHudOwnedPtr* value) {
    self->EmplaceBack(static_cast<UnkHudOwnedPtr&&>(*value));
}
