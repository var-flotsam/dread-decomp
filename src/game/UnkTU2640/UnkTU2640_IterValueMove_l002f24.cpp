// Unit 2640: the move assignment of the iterator value over a keyed collection of (string id, record) pairs,
// each record a string id and an array of 0x78-byte elements. Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CStrId.h"
#include "game/GUI/UnkMissionLogRecords.h"
#include "game/UnkElem78.h"
#include "game/UnkTU2640.h"
#include "types.h"

using base::global::CStrId;

// Takes over the other array's buffer, emptying it (nothing happens on a self-move).
inline void MoveItems_l002f24(UnkElemArray<UnkElem78>& dst, UnkElemArray<UnkElem78>& src) {
    if (&dst != &src) {
        fn_71010017f4(&dst);
        dst.allocator = src.allocator;
        dst.flags = src.flags;
        dst.step = src.step;
        dst.field_0x14 = src.field_0x14;
        dst.count = src.count;
        dst.capacity = src.capacity;
        dst.data = src.data;
        src.data = nullptr;
        src.count = 0;
        src.capacity = 0;
    }
}

// A string id and an array, moved member by member.
struct UnkIdArray78Entry_l002f24 {
    CStrId id;
    UnkElemArray<UnkElem78> items;  // 0x8

    UnkIdArray78Entry_l002f24& operator=(UnkIdArray78Entry_l002f24&& other) {
        id.ReleaseAndTakeReloading(other.id);
        MoveItems_l002f24(items, other.items);
        return *this;
    }
};
static_assert(sizeof(UnkIdArray78Entry_l002f24) == 0x30, "UnkIdArray78Entry_l002f24 layout");

using UnkIdArray78Value_l002f24 = base::UnkIterValue<UnkIdArray78Entry_l002f24, CStrId>;

// Moves the pair `other` holds or refers to into the one `self` holds or refers to.
UnkIdArray78Value_l002f24* fn_7101002f24(UnkIdArray78Value_l002f24* self, UnkIdArray78Value_l002f24* other) {
    if (other->state == 1) {
        self->Index().ReleaseAndTakeReloading(other->value.index);
        self->Element() = static_cast<UnkIdArray78Entry_l002f24&&>(other->value.element);
    } else if (other->state == 2) {
        CStrId& key = *const_cast<CStrId*>(other->ref.index);
        self->Index().ReleaseAndTakeReloading(key);
        self->Element() = static_cast<UnkIdArray78Entry_l002f24&&>(*other->ref.element);
    }
    return self;
}
