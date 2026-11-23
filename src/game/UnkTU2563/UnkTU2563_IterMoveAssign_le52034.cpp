// CMinimapData's unit (2563): the body of the move assignment of an iterator value over records of two
// arrays and four words indexed by u32 (its caller checks for self-assignment). Names and layouts are
// placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CRntVector.h"
#include "game/UnkArrayPairRecord_le53cac.h"
#include "types.h"

static_assert(sizeof(UnkArrayPairRecord_le53cac) == 0x60, "UnkArrayPairRecord_le53cac layout");

typedef base::UnkIterValue<UnkArrayPairRecord_le53cac, u32> UnkArrayPairIndexIterValue_le52034;

void fn_7100e52034(UnkArrayPairIndexIterValue_le52034* self, UnkArrayPairIndexIterValue_le52034* other) {
    if (other->state == 1) {
        u32 index = other->value.index;
        self->Index() = index;
        self->Element() = static_cast<UnkArrayPairRecord_le53cac&&>(other->value.element);
    } else if (other->state == 2) {
        u32 index = *other->ref.index;
        self->Index() = index;
        self->Element() = static_cast<UnkArrayPairRecord_le53cac&&>(*other->ref.element);
    }
}
