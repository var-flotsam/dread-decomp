// UnkTU2523 (tools/tu_map.py unit 2523): the move assignment of an iterator value over a collection of arrays
// of 0x40-byte polymorphic elements keyed by string ids. Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// A 0x40-byte element destroyed through its virtual destructor.
struct UnkElem40_ld8945c {
    virtual ~UnkElem40_ld8945c();
    u8 pad_0x8[0x38];
};
static_assert(sizeof(UnkElem40_ld8945c) == 0x40, "UnkElem40_ld8945c layout");

using UnkElem40Array_ld8945c = base::global::CRntVector<UnkElem40_ld8945c>;
using UnkElem40ArrayIdValue_ld8945c = base::UnkIterValue<UnkElem40Array_ld8945c, CStrId>;

// Moves the (id, array) pair `other` holds or refers to into the one `self` holds or refers to.
UnkElem40ArrayIdValue_ld8945c* fn_7100d8945c(UnkElem40ArrayIdValue_ld8945c* self,
                                             UnkElem40ArrayIdValue_ld8945c* other) {
    if (other->state == 1) {
        self->Index().ReleaseAndTakeReloading(other->value.index);
        self->Element() = static_cast<UnkElem40Array_ld8945c&&>(other->value.element);
    } else if (other->state == 2) {
        CStrId& key = *const_cast<CStrId*>(other->ref.index);
        self->Index().ReleaseAndTakeReloading(key);
        self->Element() = static_cast<UnkElem40Array_ld8945c&&>(*other->ref.element);
    }
    return self;
}
