// CMinimapData's unit (2563): the move assignment of an iterator value over small dictionaries of records
// of two arrays and four words, keyed by string ids. Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// Two arrays and four words; destroying it frees both arrays, the second first.
struct UnkArrayPairEntry_le57768 {
    base::global::CRntVector<u32> first;
    base::global::CRntVector<u32> second;
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
    u32 field_0x5c;

    ~UnkArrayPairEntry_le57768() {
        second.Free();
        first.Free();
    }
};
static_assert(sizeof(UnkArrayPairEntry_le57768) == 0x60, "UnkArrayPairEntry_le57768 layout");

using UnkEntryDict_le57768 = base::global::CRntSmallDictionary<u64, UnkArrayPairEntry_le57768>;
using UnkEntryDictIdValue_le57768 = base::UnkIterValue<UnkEntryDict_le57768, CStrId>;

// Moves `other` into `self`: the dictionary's move assignment as this unit has it, with no check for
// assignment to itself.
inline void MoveDict_le57768(UnkEntryDict_le57768& other, UnkEntryDict_le57768& self) {
    self.Clear();
    self.allocator = other.allocator;
    self.keys = other.keys;
    self.values = other.values;
    self.capacity = other.capacity;
    self.count = other.count;
    self.flags = other.flags;
    self.step = other.step;
    other.count = 0;
    other.values = nullptr;
    other.keys = nullptr;
}

// Moves the (id, dictionary) pair `other` holds or refers to into the one `self` holds or refers to.
UnkEntryDictIdValue_le57768* fn_7100e57768(UnkEntryDictIdValue_le57768* self, UnkEntryDictIdValue_le57768* other) {
    if (other->state == 1) {
        self->Index().ReleaseAndTakeReloading(other->value.index);
        MoveDict_le57768(other->value.element, self->Element());
    } else if (other->state == 2) {
        CStrId& key = *const_cast<CStrId*>(other->ref.index);
        self->Index().ReleaseAndTakeReloading(key);
        MoveDict_le57768(*other->ref.element, self->Element());
    }
    return self;
}
