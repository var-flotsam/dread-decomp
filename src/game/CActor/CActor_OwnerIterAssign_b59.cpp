// CActor's unit (tools/tu_map.py unit 341): the move assignment (reflection slot 0x50 of
// class_descriptors_00.cpp) of a cached iterator value (base::UnkIterValue, base/UnkCollectionIter.h)
// over an array of owning pointers (CActor_OwnerArrayIter_b49.cpp's element): the index is copied and
// the owned object taken over; the object replaced is told first (vf_0x90) and then deleted. Names
// are placeholders.
#include "base/UnkCollectionIter.h"
#include "types.h"

struct UnkActorOwnedObject_b59 {
    virtual void vf_0x0();
    virtual ~UnkActorOwnedObject_b59();
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
    virtual void vf_0x90();  // told before it is deleted
};

struct UnkActorOwner_b59 {
    UnkActorOwnedObject_b59* ptr;

    UnkActorOwner_b59& operator=(UnkActorOwner_b59&& other) {
        UnkActorOwnedObject_b59* p = other.ptr;
        other.ptr = nullptr;
        UnkActorOwnedObject_b59* old = ptr;
        ptr = p;
        if (old) {
            old->vf_0x90();
            delete old;
        }
        return *this;
    }
};

using UnkActorOwnerIterValue_b59 = base::UnkIterValue<UnkActorOwner_b59>;

__attribute__((flatten)) void* fn_7100243450(void* dst, const void* src) {
    return &(*static_cast<UnkActorOwnerIterValue_b59*>(dst) =
                 static_cast<UnkActorOwnerIterValue_b59&&>(*static_cast<UnkActorOwnerIterValue_b59*>(const_cast<void*>(src))));
}
