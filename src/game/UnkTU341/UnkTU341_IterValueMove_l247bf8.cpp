// Unit 341 (CActor): the move assignment of an iterator value over a collection of arrays of 0x40-byte
// polymorphic elements keyed by string ids. Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// A 0x40-byte element destroyed through its virtual destructor.
struct UnkElem40_l247bf8 {
    virtual ~UnkElem40_l247bf8();
    u8 pad_0x8[0x38];
};
static_assert(sizeof(UnkElem40_l247bf8) == 0x40, "UnkElem40_l247bf8 layout");

using UnkElem40Array_l247bf8 = base::global::CRntVector<UnkElem40_l247bf8>;
using UnkElem40ArrayIdValue_l247bf8 = base::UnkIterValue<UnkElem40Array_l247bf8, CStrId>;

// Moves the (id, array) pair `other` holds or refers to into the one `self` holds or refers to.
UnkElem40ArrayIdValue_l247bf8* fn_7100247bf8(UnkElem40ArrayIdValue_l247bf8* self,
                                             UnkElem40ArrayIdValue_l247bf8* other) {
    if (other->state == 1) {
        self->Index().ReleaseAndTakeReloading(other->value.index);
        self->Element() = static_cast<UnkElem40Array_l247bf8&&>(other->value.element);
    } else if (other->state == 2) {
        CStrId& key = *const_cast<CStrId*>(other->ref.index);
        self->Index().ReleaseAndTakeReloading(key);
        self->Element() = static_cast<UnkElem40Array_l247bf8&&>(*other->ref.element);
    }
    return self;
}
