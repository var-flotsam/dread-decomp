// CMinimapData's unit (2563): the move assignment of an iterator value over records of two arrays and four words
// keyed by string ids (no self-check). Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkArrayPairRecord_le53cac.h"
#include "types.h"

using base::global::CStrId;

static_assert(sizeof(UnkArrayPairRecord_le53cac) == 0x60, "UnkArrayPairRecord_le53cac layout");

typedef base::UnkIterValue<UnkArrayPairRecord_le53cac, CStrId> UnkArrayPairIdIterValue_le558f8;

// Moves the (id, record) pair `other` holds or refers to into the one `self` holds or refers to.
UnkArrayPairIdIterValue_le558f8* fn_7100e558f8(UnkArrayPairIdIterValue_le558f8* self,
                                               UnkArrayPairIdIterValue_le558f8* other) {
    if (other->state == 1) {
        self->Index().ReleaseAndTakeReloading(other->value.index);
        self->Element() = static_cast<UnkArrayPairRecord_le53cac&&>(other->value.element);
    } else if (other->state == 2) {
        CStrId& key = *const_cast<CStrId*>(other->ref.index);
        self->Index().ReleaseAndTakeReloading(key);
        self->Element() = static_cast<UnkArrayPairRecord_le53cac&&>(*other->ref.element);
    }
    return self;
}
