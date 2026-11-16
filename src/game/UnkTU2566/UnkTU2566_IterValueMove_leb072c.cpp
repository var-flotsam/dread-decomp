// CTunableMinimapManager's unit (2566): the move assignment of the iterator value over a keyed collection of
// (string id, vector of id-keyed entries) pairs. Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// A string id and an 8-byte value.
struct UnkIdEntry_leb072c {
    base::UnkStrIdMember id;
    u64 value;
};
using UnkIdEntryVec_leb072c = base::global::CRntVector<UnkIdEntry_leb072c>;
using UnkIdEntryVecValue_leb072c = base::UnkIterValue<UnkIdEntryVec_leb072c, CStrId>;

// Moves the pair `other` holds or refers to into the one `self` holds or refers to.
UnkIdEntryVecValue_leb072c* fn_7100eb072c(UnkIdEntryVecValue_leb072c* self, UnkIdEntryVecValue_leb072c* other) {
    if (other->state == 1) {
        self->Index().ReleaseAndTakeReloading(other->value.index);
        self->Element() = static_cast<UnkIdEntryVec_leb072c&&>(other->value.element);
    } else if (other->state == 2) {
        CStrId& key = *const_cast<CStrId*>(other->ref.index);
        self->Index().ReleaseAndTakeReloading(key);
        self->Element() = static_cast<UnkIdEntryVec_leb072c&&>(*other->ref.element);
    }
    return self;
}
