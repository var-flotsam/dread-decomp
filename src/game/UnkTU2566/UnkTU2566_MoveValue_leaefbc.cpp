// Unit 2566 (CMinimapManager::CTunableMinimapManager): moves the (index, entry) pair an iterator
// value holds or refers to into another, for an array of string ids each with two words.
// Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// A string id with two words.
struct UnkMmIdWords_leaefbc {
    CStrId id;
    u32 field_0x8;
    u32 field_0xc;

    UnkMmIdWords_leaefbc& operator=(UnkMmIdWords_leaefbc&& other) {
        id.ReleaseAndTake(other.id);
        field_0x8 = other.field_0x8;
        field_0xc = other.field_0xc;
        return *this;
    }
};

using UnkMmIdWordsValue_leaefbc = base::UnkIterValue<UnkMmIdWords_leaefbc, u32>;

UnkMmIdWordsValue_leaefbc* fn_7100eaefbc(UnkMmIdWordsValue_leaefbc* self, UnkMmIdWordsValue_leaefbc* other) {
    if (other->state == 1) {
        self->Index() = other->value.index;
        self->Element() = static_cast<UnkMmIdWords_leaefbc&&>(other->value.element);
    } else if (other->state == 2) {
        self->Index() = *other->ref.index;
        UnkMmIdWords_leaefbc& src = *other->ref.element;
        self->Element() = static_cast<UnkMmIdWords_leaefbc&&>(src);
    }
    return self;
}
