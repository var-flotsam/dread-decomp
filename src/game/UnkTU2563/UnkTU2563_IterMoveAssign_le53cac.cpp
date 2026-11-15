// CMinimapData's unit (2563): the body of the move assignment of an iterator value over records of two
// arrays and four words keyed by two-word keys (its caller checks for self-assignment). Names and layouts are
// placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CRntVector.h"
#include "types.h"

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

// The records' key: two words (copied together as one 64-bit move).
struct UnkRecordKey_le53cac {
    s32 field_0x0;
    s32 field_0x4;
};

typedef base::UnkIterValue<UnkArrayPairRecord_le53cac, UnkRecordKey_le53cac> UnkArrayPairIterValue_le53cac;

void fn_7100e53cac(UnkArrayPairIterValue_le53cac* self, UnkArrayPairIterValue_le53cac* other) {
    if (other->state == 1) {
        UnkRecordKey_le53cac key = other->value.index;
        self->Index() = key;
        self->Element() = static_cast<UnkArrayPairRecord_le53cac&&>(other->value.element);
    } else if (other->state == 2) {
        UnkRecordKey_le53cac key = *other->ref.index;
        self->Index() = key;
        self->Element() = static_cast<UnkArrayPairRecord_le53cac&&>(*other->ref.element);
    }
}
