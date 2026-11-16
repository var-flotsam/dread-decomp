// CMinimapDef's unit (2564): the move assignment of an iterator value over records of a string id and four
// words, keyed by string ids (no self-check). Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// A string id and four words.
struct UnkIdRecord_le70c4c {
    CStrId id;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;

    // Takes the other's id and copies its words.
    void MoveFrom(UnkIdRecord_le70c4c& other) {
        id.ReleaseAndTake(other.id);
        field_0x8 = other.field_0x8;
        field_0xc = other.field_0xc;
        field_0x10 = other.field_0x10;
        field_0x14 = other.field_0x14;
    }
};
static_assert(sizeof(UnkIdRecord_le70c4c) == 0x18, "UnkIdRecord_le70c4c layout");

typedef base::UnkIterValue<UnkIdRecord_le70c4c, CStrId> UnkIdRecordIterValue_le70c4c;

// Moves the (id, record) pair `other` holds or refers to into the one `self` holds or refers to.
UnkIdRecordIterValue_le70c4c* fn_7100e70c4c(UnkIdRecordIterValue_le70c4c* self, UnkIdRecordIterValue_le70c4c* other) {
    if (other->state == 1) {
        self->Index().ReleaseAndTakeReloading(other->value.index);
        self->Element().MoveFrom(other->value.element);
    } else if (other->state == 2) {
        CStrId& key = *const_cast<CStrId*>(other->ref.index);
        self->Index().ReleaseAndTakeReloading(key);
        UnkIdRecord_le70c4c& from = *other->ref.element;
        self->Element().MoveFrom(from);
    }
    return self;
}
