// Unit 2640: the move helper of the iterator value over a reflected collection of (string id, record) pairs:
// the ids become the empty string and the record's array gives up its buffer. Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/UnkTU2640Array.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkTU2640Record {
    UnkMovedStrId id;
    UnkTU2640Array array;  // 0x8

    UnkTU2640Record(UnkTU2640Record&& other) = default;
};

void fn_7101002be8(void* dst, const void* src) {
    base::refl_IterValueMove<UnkTU2640Record, UnkMovedStrId>(dst, src);
}
