// Unit 2640: the move assignment of the iterator value over a keyed collection of (string id, record) pairs,
// each record an array of 0x50-byte records and a string id. Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CStrId.h"
#include "types.h"
#include "../UnkUnit2601/UnkUnit2601_VecCopy_b56.h"

using base::global::CStrId;

// An array of records and a string id, moved member by member.
struct UnkRecordsIdEntry_lfff488 {
    UnkRecord50Vec_b56 records;
    CStrId id;  // 0x28

    UnkRecordsIdEntry_lfff488& operator=(UnkRecordsIdEntry_lfff488&& other) {
        if (&records != &other.records) {
            fn_7100d90460(&records, 0);
            records.allocator = other.records.allocator;
            records.flags = other.records.flags;
            records.step = other.records.step;
            records.field_0x14 = other.records.field_0x14;
            records.count = other.records.count;
            records.capacity = other.records.capacity;
            records.data = other.records.data;
            other.records.data = nullptr;
            other.records.count = 0;
            other.records.capacity = 0;
        }
        id.ReleaseAndTake(other.id);
        return *this;
    }
};
static_assert(sizeof(UnkRecordsIdEntry_lfff488) == 0x30, "UnkRecordsIdEntry_lfff488 layout");

using UnkRecordsIdValue_lfff488 = base::UnkIterValue<UnkRecordsIdEntry_lfff488, CStrId>;

// Moves the pair `other` holds or refers to into the one `self` holds or refers to.
UnkRecordsIdValue_lfff488* fn_7100fff488(UnkRecordsIdValue_lfff488* self, UnkRecordsIdValue_lfff488* other) {
    if (other->state == 1) {
        self->Index().ReleaseAndTakeReloading(other->value.index);
        self->Element() = static_cast<UnkRecordsIdEntry_lfff488&&>(other->value.element);
    } else if (other->state == 2) {
        CStrId& key = *const_cast<CStrId*>(other->ref.index);
        self->Index().ReleaseAndTakeReloading(key);
        self->Element() = static_cast<UnkRecordsIdEntry_lfff488&&>(*other->ref.element);
    }
    return self;
}
