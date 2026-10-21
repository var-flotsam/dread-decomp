// Destructors (complete and deleting) of an object holding an array of polymorphic 0x18-byte
// elements: restore the vtable, destroy the elements through their vtables and free the array
// (the deleting one then frees the object).
#include <new>

#include "base/UnkVector.h"
#include "types.h"

struct UnkPolyElem_0x18 {
    virtual ~UnkPolyElem_0x18();
    u64 field_0x8;
    u64 field_0x10;
};

extern void* g_unk_7101caa380[];

struct UnkPolyArrayOwner_7100f03c94 {
    void** vtable;
    u8 unk_0x8[0x18];
    base::UnkVector<UnkPolyElem_0x18> elems;

    ~UnkPolyArrayOwner_7100f03c94() {
        vtable = &g_unk_7101caa380[2];
        elems.Free();
    }
};

void fn_7100f03be8(UnkPolyArrayOwner_7100f03c94& self) {
    self.~UnkPolyArrayOwner_7100f03c94();
}

void fn_7100f03c94(UnkPolyArrayOwner_7100f03c94& self) {
    delete &self;
}
