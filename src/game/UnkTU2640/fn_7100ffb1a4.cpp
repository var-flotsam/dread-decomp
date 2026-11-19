// Unit 2640: moves the (index, entry) pair one iterator value holds or refers to into another, for an
// array of 0x18-byte entries that begin with a file-path id. Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::CFilePathStrId;

// A file-path id with a word and a half-word.
struct UnkTU2640PathEntry_lffb1a4 {
    CFilePathStrId path;
    u64 field_0x8;
    u32 field_0x10;

    UnkTU2640PathEntry_lffb1a4& operator=(UnkTU2640PathEntry_lffb1a4&& other) {
        path = static_cast<CFilePathStrId&&>(other.path);
        field_0x8 = other.field_0x8;
        field_0x10 = other.field_0x10;
        return *this;
    }
};

using UnkTU2640PathIterValue_lffb1a4 = base::UnkIterValue<UnkTU2640PathEntry_lffb1a4, u32>;

// NON_MATCHING(functional): Calls differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
UnkTU2640PathIterValue_lffb1a4* fn_7100ffb1a4(UnkTU2640PathIterValue_lffb1a4* self,
                                              UnkTU2640PathIterValue_lffb1a4* other) {
    if (other->state == 1) {
        self->Index() = other->value.index;
        self->Element() = static_cast<UnkTU2640PathEntry_lffb1a4&&>(other->value.element);
    } else if (other->state == 2) {
        self->Index() = *other->ref.index;
        UnkTU2640PathEntry_lffb1a4& src = *other->ref.element;
        self->Element() = static_cast<UnkTU2640PathEntry_lffb1a4&&>(src);
    }
    return self;
}
