// Destructors (complete and deleting) of an object with an array of 0x98-byte entries at 0x30:
// restore the vtable, destroy the entries (fn_71011ad580), free the array and run the base
// destructor (fn_710025373c); the deleting one then frees the object.
#include "base/UnkVector.h"
#include "types.h"

struct UnkBase_0x30 {
    void** vtable;
    u8 unk_0x8[0x28];
    ~UnkBase_0x30();  // fn_710025373c
};

struct UnkEntry_0x98 {
    u8 unk_0x0[0x98];
    ~UnkEntry_0x98();  // fn_71011ad580
};

extern void* g_unk_7101cb5818[];

struct UnkObject_71011ace00 : UnkBase_0x30 {
    union {
        base::UnkVector<UnkEntry_0x98> entries;
    };

    ~UnkObject_71011ace00() {
        vtable = &g_unk_7101cb5818[2];
        entries.Clear();
        entries.Free();
    }
};

void fn_71011acd74(UnkObject_71011ace00& self) {
    self.~UnkObject_71011ace00();
}

void fn_71011ace00(UnkObject_71011ace00& self) {
    delete &self;
}
