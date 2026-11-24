// Destructors (complete and deleting) of an object with an array of owned objects at 0x58 and a
// table at 0x80: restore the vtable, clear the table (fn_71010287d4) and free its two arrays,
// delete the owned objects (fn_710102796c) and free their array, then run the base destructor
// (fn_7100016f88); the deleting one then frees the object.
#include "base/global/CRntVector.h"
#include "game/UnkBase_0x58.h"
#include "game/sound/CSoundManager.h"
#include "types.h"

struct UnkOwnedObj_7101025a9c {
    u8 unk_0x0[0x8];
    ~UnkOwnedObj_7101025a9c();  // fn_710102796c
};

// An owning pointer: cleared, then its object deleted.
struct UnkOwnedPtr_01025a9c {
    // A union member (no type-based alias info): the array is reloaded after the pointer is
    // cleared.
    union {
        UnkOwnedObj_7101025a9c* ptr;
    };

    ~UnkOwnedPtr_01025a9c() {
        UnkOwnedObj_7101025a9c* p = ptr;
        ptr = nullptr;
        if (p)
            delete p;
    }
};

struct UnkTable_0x28_01025a9c;
void fn_71010287d4(UnkTable_0x28_01025a9c* self);

struct UnkTable_0x28_01025a9c {
    void* field_0x0;
    void* field_0x8;
    u8 unk_0x10[0x10];
    base::IAllocator* allocator;

    ~UnkTable_0x28_01025a9c() {
        fn_71010287d4(this);
        if (field_0x8)
            allocator->vf_0x48(field_0x8);
        if (field_0x0)
            allocator->vf_0x48(field_0x0);
    }
};

struct UnkObject_7101025a9c : UnkBase_0x58 {
    union {
        base::global::CRntVector<UnkOwnedPtr_01025a9c> objects;
    };
    // Destroyed before the array above, so both are destroyed by hand.
    union {
        UnkTable_0x28_01025a9c table;
    };

    __attribute__((always_inline)) ~UnkObject_7101025a9c() {
        vtable = &g_unk_7101cac688[2];
        table.~UnkTable_0x28_01025a9c();
        objects.Free();
    }
};

void fn_71010259b0(UnkObject_7101025a9c& self) {
    self.~UnkObject_7101025a9c();
}

void fn_7101025a9c(UnkObject_7101025a9c& self) {
    delete &self;
}
