// CMinimapData's unit (2563): the body of the move assignment of an iterator value over records of two
// arrays and four words keyed by two-word keys (its caller checks for self-assignment). Names and layouts are
// placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CRntVector.h"
#include "game/UnkArrayPairRecord_le53cac.h"
#include "types.h"

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
