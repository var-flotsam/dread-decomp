// Destructors (complete and deleting) of an object holding a string id and an array of owned objects: restores
// the vtable, deletes the owned objects through their vtables (slot 2) and frees the array,
// releases the id (the deleting one then frees the object).
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkOwnedObj {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // deletes the object
};

// An owning pointer: cleared, then its object deleted.
struct UnkOwnedPtr_fb4f70 {
    // A union member (no type-based alias info): the array's count is reloaded after the
    // pointer is cleared.
    union {
        UnkOwnedObj* ptr;
    };

    ~UnkOwnedPtr_fb4f70() {
        UnkOwnedObj* p = ptr;
        ptr = nullptr;
        if (p)
            p->vf_0x10();
    }
};

extern void* g_unk_7101cab468[];

struct UnkOwner_7100fb4f70 {
    void** vtable;
    UnkStrIdTwoStep id;
    base::global::CRntVector<UnkOwnedPtr_fb4f70> objects;

    ~UnkOwner_7100fb4f70() {
        vtable = &g_unk_7101cab468[2];
        objects.Clear();
        objects.Free();
    }
};

void fn_7100fb4e90(UnkOwner_7100fb4f70& self) {
    self.~UnkOwner_7100fb4f70();
}

void fn_7100fb4f70(UnkOwner_7100fb4f70& self) {
    delete &self;
}
