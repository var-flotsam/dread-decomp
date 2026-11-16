// Unit 2604: the move assignment of the iterator value over records of a string id and a small block of
// plain data, keyed by string ids (no self-check). Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// Five words copied as one.
struct UnkIdData_lf9197c {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
};

// A string id and its data.
struct UnkIdRecord_lf9197c {
    CStrId id;
    UnkIdData_lf9197c data;  // 0x8
};
static_assert(sizeof(UnkIdRecord_lf9197c) == 0x20, "UnkIdRecord_lf9197c layout");

typedef base::UnkIterValue<UnkIdRecord_lf9197c, CStrId> UnkIdRecordIterValue_lf9197c;

// Moves the (id, record) pair `other` holds or refers to into the one `self` holds or refers to.
UnkIdRecordIterValue_lf9197c* fn_7100f9197c(UnkIdRecordIterValue_lf9197c* self, UnkIdRecordIterValue_lf9197c* other) {
    if (other->state == 1) {
        self->Index().ReleaseAndTakeReloading(other->value.index);
        UnkIdRecord_lf9197c& element = self->Element();
        element.id.ReleaseAndTake(other->value.element.id);
        element.data = other->value.element.data;
    } else if (other->state == 2) {
        CStrId& key = *const_cast<CStrId*>(other->ref.index);
        self->Index().ReleaseAndTakeReloading(key);
        UnkIdRecord_lf9197c& from = *other->ref.element;
        UnkIdRecord_lf9197c& element = self->Element();
        element.id.ReleaseAndTake(from.id);
        element.data = from.data;
    }
    return self;
}
