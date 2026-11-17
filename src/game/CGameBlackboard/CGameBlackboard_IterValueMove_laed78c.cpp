// CGameBlackboard's unit (1647): the move assignment of the iterator value over a keyed collection of
// (string id, keyed string-id table) pairs. Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/UnkKeyedStrIds.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// The keyed string-id table as this unit's collection holds it, with its move assignment.
struct UnkKeyedIdTable_laed78c : UnkKeyedStrIds {
    // Releases every id; the arrays are kept.
    void Clear() {
        if (ids) {
            for (u32 i = 0; i < count; ++i)
                ids[i].~UnkStrIdMemberOf();
        }
        count = 0;
    }

    // Takes over other's arrays, leaving it empty.
    UnkKeyedIdTable_laed78c& operator=(UnkKeyedIdTable_laed78c&& other) {
        Clear();
        allocator = other.allocator;
        ids = other.ids;
        keys = other.keys;
        capacity = other.capacity;
        count = other.count;
        field_0x1c = other.field_0x1c;
        field_0x18 = other.field_0x18;
        other.count = 0;
        other.keys = nullptr;
        other.ids = nullptr;
        return *this;
    }
};
static_assert(sizeof(UnkKeyedIdTable_laed78c) == 0x28, "UnkKeyedIdTable_laed78c layout");

using UnkKeyedIdTableValue_laed78c = base::UnkIterValue<UnkKeyedIdTable_laed78c, CStrId>;

// Moves the pair `other` holds or refers to into the one `self` holds or refers to.
UnkKeyedIdTableValue_laed78c* fn_7100aed78c(UnkKeyedIdTableValue_laed78c* self, UnkKeyedIdTableValue_laed78c* other) {
    if (other->state == 1) {
        self->Index().ReleaseAndTakeReloading(other->value.index);
        self->Element() = static_cast<UnkKeyedIdTable_laed78c&&>(other->value.element);
    } else if (other->state == 2) {
        CStrId& key = *const_cast<CStrId*>(other->ref.index);
        self->Index().ReleaseAndTakeReloading(key);
        self->Element() = static_cast<UnkKeyedIdTable_laed78c&&>(*other->ref.element);
    }
    return self;
}
