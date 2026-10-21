// Destructors (complete and deleting) of an object holding an array of polymorphic 0x20-byte
// elements: restore the vtable, destroy the elements through their vtables (slot 1) and free
// the array (the deleting one then frees the object).
#include <new>

#include "base/UnkVector.h"
#include "types.h"

struct UnkPolyElem_0x20 {
    virtual void vf_0x0();
    virtual ~UnkPolyElem_0x20();
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
};

extern void* g_unk_7101cac938[];

struct UnkPolyArrayOwner_710105a224 {
    void** vtable;
    base::UnkVector<UnkPolyElem_0x20> elems;

    ~UnkPolyArrayOwner_710105a224() {
        vtable = &g_unk_7101cac938[2];
        elems.Free();
    }
};

void fn_710105a178(UnkPolyArrayOwner_710105a224& self) {
    self.~UnkPolyArrayOwner_710105a224();
}

void fn_710105a224(UnkPolyArrayOwner_710105a224& self) {
    delete &self;
}
