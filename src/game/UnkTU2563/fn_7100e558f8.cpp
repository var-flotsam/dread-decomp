// CMinimapData's unit (2563): the move assignment of an iterator value over records of two arrays and four words
// keyed by string ids (no self-check). Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// Two arrays and four words, moved member by member.
struct UnkArrayPairRecord_le53cac {
    base::global::CRntVector<u32> first;
    base::global::CRntVector<u32> second;
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
    u32 field_0x5c;

    UnkArrayPairRecord_le53cac& operator=(UnkArrayPairRecord_le53cac&& other) {
        first = static_cast<base::global::CRntVector<u32>&&>(other.first);
        second = static_cast<base::global::CRntVector<u32>&&>(other.second);
        field_0x50 = other.field_0x50;
        field_0x54 = other.field_0x54;
        field_0x58 = other.field_0x58;
        field_0x5c = other.field_0x5c;
        return *this;
    }
};
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
