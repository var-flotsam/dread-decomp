// CMinimapData's unit (2563): the move assignment of an iterator value over records of three arrays and a
// bounding box keyed by string ids (no self-check). Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// Three arrays with a bounding box; assigning one marks the box dirty and leaves it as it was.
struct UnkTripleArrayRecord_le5e728 {
    base::global::CRntVector<u32> first;
    u32 field_0x28;
    u8 field_0x2c;
    bool dirty;
    f32 min[2];
    f32 max[2];
    u32 field_0x40;
    base::global::CRntVector<u32> second;
    base::global::CRntVector<u32> third;
    u32 field_0x98;
    u32 field_0x9c;
    u32 field_0xa0;
    u32 field_0xa4;

    inline UnkTripleArrayRecord_le5e728& operator=(UnkTripleArrayRecord_le5e728&& other) {
        field_0x28 = other.field_0x28;
        field_0x2c = other.field_0x2c;
        dirty = true;
        first = static_cast<base::global::CRntVector<u32>&&>(other.first);
        field_0x40 = other.field_0x40;
        second = static_cast<base::global::CRntVector<u32>&&>(other.second);
        third = static_cast<base::global::CRntVector<u32>&&>(other.third);
        field_0x98 = other.field_0x98;
        field_0x9c = other.field_0x9c;
        field_0xa0 = other.field_0xa0;
        field_0xa4 = other.field_0xa4;
        return *this;
    }
};
static_assert(sizeof(UnkTripleArrayRecord_le5e728) == 0xa8, "UnkTripleArrayRecord_le5e728 layout");

typedef base::UnkIterValue<UnkTripleArrayRecord_le5e728, CStrId> UnkTripleArrayIdIterValue_le5e728;

// Moves the (id, record) pair `other` holds or refers to into the one `self` holds or refers to.
UnkTripleArrayIdIterValue_le5e728* fn_7100e5e728(UnkTripleArrayIdIterValue_le5e728* self,
                                                 UnkTripleArrayIdIterValue_le5e728* other) {
    if (other->state == 1) {
        self->Index().ReleaseAndTakeReloading(other->value.index);
        self->Element() = static_cast<UnkTripleArrayRecord_le5e728&&>(other->value.element);
    } else if (other->state == 2) {
        CStrId& key = *const_cast<CStrId*>(other->ref.index);
        self->Index().ReleaseAndTakeReloading(key);
        self->Element() = static_cast<UnkTripleArrayRecord_le5e728&&>(*other->ref.element);
    }
    return self;
}
