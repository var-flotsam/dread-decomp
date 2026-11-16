// Unit 341 (CActor): the move assignment of an iterator value over a collection of arrays of reference-counted
// object pointers keyed by string ids. Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// A reference-counted object, destroyed through vf_0x10 when its last reference goes.
struct UnkRefCounted_l244744 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    u32 ref_count;
};

// Drops one reference, destroying the object when it was the last.
inline void ReleaseRef_l244744(UnkRefCounted_l244744* object) {
    u32& count = object->ref_count;
    if (--count == 0)
        object->vf_0x10();
}

// A counted reference to one of those objects.
struct UnkRef_l244744 {
    UnkRefCounted_l244744* ptr;

    ~UnkRef_l244744() {
        if (ptr)
            ReleaseRef_l244744(ptr);
    }
};

using UnkRefArray_l244744 = base::global::CRntVector<UnkRef_l244744>;
using UnkRefArrayIdValue_l244744 = base::UnkIterValue<UnkRefArray_l244744, CStrId>;

// Moves the (id, array) pair `other` holds or refers to into the one `self` holds or refers to.
UnkRefArrayIdValue_l244744* fn_7100244744(UnkRefArrayIdValue_l244744* self, UnkRefArrayIdValue_l244744* other) {
    if (other->state == 1) {
        self->Index().ReleaseAndTakeReloading(other->value.index);
        self->Element() = static_cast<UnkRefArray_l244744&&>(other->value.element);
    } else if (other->state == 2) {
        CStrId& key = *const_cast<CStrId*>(other->ref.index);
        self->Index().ReleaseAndTakeReloading(key);
        self->Element() = static_cast<UnkRefArray_l244744&&>(*other->ref.element);
    }
    return self;
}
