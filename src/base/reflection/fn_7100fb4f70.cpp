// Destructors (complete and deleting) of an object holding a string id and an array of owned objects: restores
// the vtable, deletes the owned objects through their vtables (slot 2) and frees the array,
// releases the id (the deleting one then frees the object).
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id whose release decrements and tests the count in two steps.
struct UnkOwnerId {
    UnkStringData* data;

    ~UnkOwnerId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkOwnedObj {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // deletes the object
};

// An owning pointer: cleared, then its object deleted.
struct UnkOwnedPtr {
    // A union member (no type-based alias info): the array's count is reloaded after the
    // pointer is cleared.
    union {
        UnkOwnedObj* ptr;
    };

    ~UnkOwnedPtr() {
        UnkOwnedObj* p = ptr;
        ptr = nullptr;
        if (p)
            p->vf_0x10();
    }
};

extern void* g_unk_7101cab468[];

struct UnkOwner_7100fb4f70 {
    void** vtable;
    UnkOwnerId id;
    base::UnkVector<UnkOwnedPtr> objects;

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
